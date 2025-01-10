const fetch = require('node-fetch');
const Boss = require('../mongoDB/bossTable');

// Nexon OpenAPI Controller
exports.getCharacterData = async (req, res) => {
    const API_KEY = process.env.NEXON_API_KEY; // 환경변수에서 API 키 가져오기
    const { characterName } = req.params; // URL에서 characterName 추출

    if (!API_KEY) {
        return res.status(500).json({ error: "API key is missing. Please set NEXON_API_KEY in .env file." });
    }

    const character_name = encodeURIComponent(characterName);
    const id_url = `https://open.api.nexon.com/maplestory/v1/id?character_name=${character_name}`;

    try {
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // 캐릭터 식별자(ocid) 조회
        const id_response  = await fetch(id_url, {
            headers: {
                "x-nxopen-api-key": API_KEY,
                "accept": "application/json",
            },
        });

        // 응답 확인
        if (!id_response .ok) {
            const errorMessage = `API call failed with status ${id_response.status}: ${id_response.statusText}`;
            console.error(errorMessage);
            return res.status(id_response.status).json({ error: errorMessage });
        }

        const id_json = await id_response.json();
        const ocid = id_json.ocid;
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // 종합 능력치 정보 조회
        const stat_url = `https://open.api.nexon.com/maplestory/v1/character/stat?ocid=${ocid}`;
        const stat_response = await fetch(stat_url, {
            headers: {
                "x-nxopen-api-key": API_KEY,
                "accept": "application/json",
            },
        });

        // 응답 확인
        if (!stat_response.ok) {
            const errorMessage = `Failed to fetch character stats with status ${stat_response.status}: ${stat_response.statusText}`;
            console.error(errorMessage);
            return res.status(stat_response.status).json({ error: errorMessage });
        }

        const stat_json = await stat_response.json();
        const battlePower = stat_json.final_stat?.find(
            (stat) => stat.stat_name === "전투력"
        )?.stat_value || "N/A";
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // 모든 보스 데이터
        let bosses = await Boss.find();

        // 보스 데이터를 cost 기준으로 내림차순 정렬
        bosses = bosses.sort((a, b) => b.cost - a.cost);

        // battlePower 조건을 만족하면서 같은 이름의 보스 중 cost가 가장 큰 보스만 선택
        const selectedBosses = [];
        const uniqueNames = new Set();

        for (const boss of bosses) {
            if (boss.battle_power <= battlePower && !uniqueNames.has(boss.name)) {
                selectedBosses.push(boss);
                uniqueNames.add(boss.name);
            }

            if (selectedBosses.length === 12) {
                break; // 최대 12개의 보스만 선택
            }
        }

        // totalCost 계산
        const totalCost = selectedBosses.reduce((sum, boss) => sum + boss.cost, 0);
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        res.status(200).json({
            message: "Character data fetched successfully",
            ocid,
            battlePower,
            selectedBosses: selectedBosses,
            totalCost,
        });

        /*
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // 기본 정보 조회
        const basic_url = `https://open.api.nexon.com/maplestory/v1/character/basic?ocid=${ocid}`;
        const basic_response  = await fetch(basic_url, {
            headers: {
                "x-nxopen-api-key": API_KEY,
                "accept": "application/json",
            },
        });

        // 응답 확인
        if (!basic_response .ok) {
            const errorMessage = `API call failed with status ${basic_response.status}: ${basic_response.statusText}`;
            console.error(errorMessage);
            return res.status(basic_response.status).json({ error: errorMessage });
        }

        const basic_json = await basic_response.json();
        const character_level = basic_json.character_level;
        const guild_name = encodeURIComponent(basic_json.character_guild_name);
        const world_name = encodeURIComponent(basic_json.world_name);
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // 길드 식별자(oguild_id) 조회
        let oguild_id;
        if (guild_name || world_name) {
            const guild_id_url = `https://open.api.nexon.com/maplestory/v1/guild/id?guild_name=${guild_name}&world_name=${world_name}`;
            const guild_id_response  = await fetch(guild_id_url, {
                headers: {
                    "x-nxopen-api-key": API_KEY,
                    "accept": "application/json",
                },
            });
            

            // 응답 확인
            if (!guild_id_response.ok) {
                const errorMessage = `API call failed with status ${guild_id_response.status}: ${guild_id_response.statusText}`;
                console.error(errorMessage);
                return res.status(guild_id_response.status).json({ error: errorMessage });
            }

            const guild_id_json = await guild_id_response.json();
            oguild_id = guild_id_json.oguild_id;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // 종합 능력치 정보 조회
        const stat_url = `https://open.api.nexon.com/maplestory/v1/character/stat?ocid=${ocid}`;
        const stat_response = await fetch(stat_url, {
            headers: {
                "x-nxopen-api-key": API_KEY,
                "accept": "application/json",
            },
        });

        // 응답 확인
        if (!stat_response.ok) {
            const errorMessage = `Failed to fetch character stats with status ${stat_response.status}: ${stat_response.statusText}`;
            console.error(errorMessage);
            return res.status(stat_response.status).json({ error: errorMessage });
        }

        const stat_json = await stat_response.json();
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // 하이퍼스탯 정보 조회
        const hyper_stat_url = `https://open.api.nexon.com/maplestory/v1/character/hyper-stat?ocid=${ocid}`;
        const hyper_stat_response = await fetch(hyper_stat_url, {
            headers: {
                "x-nxopen-api-key": API_KEY,
                "accept": "application/json",
            },
        });

        // 응답 확인
        if (!hyper_stat_response.ok) {
            const errorMessage = `Failed to fetch character stats with status ${hyper_stat_response.status}: ${hyper_stat_response.statusText}`;
            console.error(errorMessage);
            return res.status(hyper_stat_response.status).json({ error: errorMessage });
        }

        const hyper_stat_json = await hyper_stat_response.json();
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // 성향 정보 조회
        const propensity_url = `https://open.api.nexon.com/maplestory/v1/character/propensity?ocid=${ocid}`;
        const propensity_response = await fetch(propensity_url, {
            headers: {
                "x-nxopen-api-key": API_KEY,
                "accept": "application/json",
            },
        });

        // 응답 확인
        if (!propensity_response.ok) {
            const errorMessage = `Failed to fetch character stats with status ${propensity_response.status}: ${propensity_response.statusText}`;
            console.error(errorMessage);
            return res.status(propensity_response.status).json({ error: errorMessage });
        }

        const propensity_json = await propensity_response.json();
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // 어빌리티 정보 조회
        const ability_url = `https://open.api.nexon.com/maplestory/v1/character/ability?ocid=${ocid}`;
        const ability_response = await fetch(ability_url, {
            headers: {
                "x-nxopen-api-key": API_KEY,
                "accept": "application/json",
            },
        });

        // 응답 확인
        if (!ability_response.ok) {
            const errorMessage = `Failed to fetch character stats with status ${ability_response.status}: ${ability_response.statusText}`;
            console.error(errorMessage);
            return res.status(ability_response.status).json({ error: errorMessage });
        }

        const ability_json = await ability_response.json();
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // 장착 장비 정보 조회 (캐시 장비 제외)
        const item_equipment_url = `https://open.api.nexon.com/maplestory/v1/character/item-equipment?ocid=${ocid}`;
        const item_equipment_response = await fetch(item_equipment_url, {
            headers: {
                "x-nxopen-api-key": API_KEY,
                "accept": "application/json",
            },
        });

        // 응답 확인
        if (!item_equipment_response.ok) {
            const errorMessage = `Failed to fetch character stats with status ${item_equipment_response.status}: ${item_equipment_response.statusText}`;
            console.error(errorMessage);
            return res.status(item_equipment_response.status).json({ error: errorMessage });
        }

        const item_equipment_json = await item_equipment_response.json();

        // LUK 값과 LUK % 값 합산 변수 초기화
        let totalEquipLuk = 0;
        let totalEquipLukPercentage = 0;

        // 모든 item_equipment 데이터 순회
        item_equipment_json.item_equipment.forEach((item) => {
            // item_total_option에서 LUK 값 추가
            const totalLukValue = parseInt(item.item_total_option.luk, 10);
            if (!isNaN(totalLukValue)) {
                console.log("item_name : " + item.item_name);
                console.log("item_total_option(luk) : " + totalLukValue);
                totalEquipLuk += totalLukValue;
            }
        
            // potential_option과 additional_potential_option의 LUK 값 추가
            const potentialOptions = [
                item.potential_option_1,
                item.potential_option_2,
                item.potential_option_3,
                item.additional_potential_option_1,
                item.additional_potential_option_2,
                item.additional_potential_option_3,
            ];
        
            potentialOptions.forEach((option) => {
                if (option) {
                    const lukMatch = option.match(/(?<!레벨 당\s*)LUK\s*:\s*\+(\d+)(?![%\d])/); // LUK 값
                    const lukPercentageMatch = option.match(/LUK\s*:\s*\+(\d+)%/); // LUK % 값
                    const levelDependentLukMatch = option.match(/(\d+)레벨 당\s*LUK\s*:\s*\+(\d+)/); // m레벨 당 LUK : +n
                    
            
                    if (lukMatch) {
                        console.log("option : " + lukMatch);
                        totalEquipLuk += parseInt(lukMatch[1], 10);
                    }
            
                    if (lukPercentageMatch) {
                        console.log("option : " + lukPercentageMatch);
                        totalEquipLukPercentage += parseInt(lukPercentageMatch[1], 10);
                    }

                    if (levelDependentLukMatch) {
                        console.log("option : " + levelDependentLukMatch);
                        const m = parseInt(levelDependentLukMatch[1], 10); // m레벨
                        const n = parseInt(levelDependentLukMatch[2], 10); // +n
                        totalEquipLuk += Math.floor(character_level / m) * n; // 합산 계산
                      }
                }
            });
        });
        
        // 결과 출력
        console.log("Total LUK (sum):", totalEquipLuk);
        console.log("Total LUK Percentage (sum):", totalEquipLukPercentage);
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // 장착 캐시 장비 정보 조회
        const cashitem_equipment_url = `https://open.api.nexon.com/maplestory/v1/character/cashitem-equipment?ocid=${ocid}`;
        const cashitem_equipment_response = await fetch(cashitem_equipment_url, {
            headers: {
                "x-nxopen-api-key": API_KEY,
                "accept": "application/json",
            },
        });

        // 응답 확인
        if (!cashitem_equipment_response.ok) {
            const errorMessage = `Failed to fetch character stats with status ${cashitem_equipment_response.status}: ${cashitem_equipment_response.statusText}`;
            console.error(errorMessage);
            return res.status(cashitem_equipment_response.status).json({ error: errorMessage });
        }

        const cashitem_equipment_json = await cashitem_equipment_response.json();
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // 장착 심볼 정보 조회
        const symbol_equipment_url = `https://open.api.nexon.com/maplestory/v1/character/symbol-equipment?ocid=${ocid}`;
        const symbol_equipment_response = await fetch(symbol_equipment_url, {
            headers: {
                "x-nxopen-api-key": API_KEY,
                "accept": "application/json",
            },
        });

        // 응답 확인
        if (!symbol_equipment_response.ok) {
            const errorMessage = `Failed to fetch character stats with status ${symbol_equipment_response.status}: ${symbol_equipment_response.statusText}`;
            console.error(errorMessage);
            return res.status(symbol_equipment_response.status).json({ error: errorMessage });
        }

        const symbol_equipment_json = await symbol_equipment_response.json();
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // 적용 세트 효과 정보 조회
        const set_effect_url = `https://open.api.nexon.com/maplestory/v1/character/set-effect?ocid=${ocid}`;
        const set_effect_response = await fetch(set_effect_url, {
            headers: {
                "x-nxopen-api-key": API_KEY,
                "accept": "application/json",
            },
        });

        // 응답 확인
        if (!set_effect_response.ok) {
            const errorMessage = `Failed to fetch character stats with status ${set_effect_response.status}: ${set_effect_response.statusText}`;
            console.error(errorMessage);
            return res.status(set_effect_response.status).json({ error: errorMessage });
        }

        const set_effect_json = await set_effect_response.json();
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // 장착 펫 정보 조회
        const pet_equipment_url = `https://open.api.nexon.com/maplestory/v1/character/pet-equipment?ocid=${ocid}`;
        const pet_equipment_response = await fetch(pet_equipment_url, {
            headers: {
                "x-nxopen-api-key": API_KEY,
                "accept": "application/json",
            },
        });

        // 응답 확인
        if (!pet_equipment_response.ok) {
            const errorMessage = `Failed to fetch character stats with status ${pet_equipment_response.status}: ${pet_equipment_response.statusText}`;
            console.error(errorMessage);
            return res.status(pet_equipment_response.status).json({ error: errorMessage });
        }

        const pet_equipment_json = await pet_equipment_response.json();
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // 스킬 정보 조회
        const skill_grades = ["0", "1", "1.5", "2", "2.5", "3", "4", "hyperpassive", "hyperactive", "5", "6"];
        let totalLuck = 0; // 모든 "행운 n 증가" 값을 더한 합계
        const skill_results = []; // 응답 데이터를 저장할 배열

        for (const grade of skill_grades) {
            const skill_url = `https://open.api.nexon.com/maplestory/v1/character/skill?ocid=${ocid}&character_skill_grade=${grade}`;
            
            try {
                const skill_response = await fetch(skill_url, {
                    headers: {
                        "x-nxopen-api-key": API_KEY,
                        "accept": "application/json",
                    },
                });

                // 응답 확인
                if (!skill_response.ok) {
                    const errorMessage = `Failed to fetch character stats for grade ${grade} with status ${skill_response.status}: ${skill_response.statusText}`;
                    console.error(errorMessage);
                    skill_results.push({ grade, error: errorMessage });
                    continue;
                }

                const skill_json = await skill_response.json();

                // 각 스킬의 "행운 n 증가"에서 n 값을 추출하여 합산
                skill_json.character_skill.forEach((skill) => {
                    
                    const skill_effect = skill.skill_effect;
                    if (skill_effect) {
                        // 정규 표현식을 사용해 "행운 n" 또는 "올스탯 n"에서 n 값 추출
                        const match = skill_effect.match(/(?:운|올스탯)\s(\d+)/);
                        if (match) {
                            //console.log("skill_grade : " + grade);
                            //console.log("skill_name : " + skill.skill_name);
                            //console.log("match : " + match);
                            const luckValue = parseInt(match[1], 10); // n 값을 숫자로 변환
                            totalLuck += luckValue; // 합산
                        }

                        // 메이플 용사
                        if (skill.skill_name === "메이플 용사") {
                            const match_ = skill_effect.match(/능력치\s(\d+)%/);
                            if (match_) {
                                // "AP 배분 LUK" 값 추출
                                const ApDistributedLuk = stat_json.final_stat.find(
                                    (stat) => stat.stat_name === "AP 배분 LUK"
                                );
                                const percentValue = parseInt(match_[1], 10) / 100;
                                const additionalValue = ApDistributedLuk.stat_value * percentValue;
                                totalLuck += additionalValue;
                                //console.log(`메이플 용사 적용: 능력치 ${percentValue * 100}%, 추가 값 ${additionalValue}`);
                            }
                        }
                    }
                });

                //console.log(`Grade ${grade} response:`, skill_json);
                skill_results.push({ grade, data: skill_json });

            } catch (error) {
                console.error(`Error fetching data for grade ${grade}:`, error.message);
                skill_results.push({ grade, error: error.message });
            }
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // 장착 링크 스킬 정보 조회
        const link_skill_url = `https://open.api.nexon.com/maplestory/v1/character/link-skill?ocid=${ocid}`;
        const link_skill_response = await fetch(link_skill_url, {
            headers: {
                "x-nxopen-api-key": API_KEY,
                "accept": "application/json",
            },
        });

        // 응답 확인
        if (!link_skill_response.ok) {
            const errorMessage = `Failed to fetch character stats with status ${link_skill_response.status}: ${link_skill_response.statusText}`;
            console.error(errorMessage);
            return res.status(link_skill_response.status).json({ error: errorMessage });
        }

        const link_skill_json = await link_skill_response.json();

        // 각 그룹의 데이터에서 "행운 n" 합계 계산
        const link_skill_groups = {
            character_link_skill: link_skill_json.character_link_skill || [],
            character_link_skill_preset_1: link_skill_json.character_link_skill_preset_1 || [],
            character_link_skill_preset_2: link_skill_json.character_link_skill_preset_2 || [],
            character_link_skill_preset_3: link_skill_json.character_link_skill_preset_3 || [],
        };

        const link_skill_results = {};

        // 각 그룹을 순회하며 "행운 n" 값 계산
        Object.entries(link_skill_groups).forEach(([groupName, preset]) => {
            let groupLuckSum = 0;
            preset.forEach((skill) => {
                const effect = skill.skill_effect;
                if (effect) {
                    const match = effect.match(/운\s(\d+)/); // "운 n" 패턴 찾기
                    if (match) {
                        //console.log("skill_name : " + skill.skill_name);
                        //console.log("skill_effect : " + skill.skill_effect);
                        groupLuckSum += parseInt(match[1], 10); // n 값을 정수로 변환하여 합산
                    }
                }
            });
            link_skill_results[groupName] = groupLuckSum; // 그룹별 합계 저장
        });

        // 최종 합계에 가장 큰 그룹 값을 추가
        totalLuck += Math.max(...Object.values(link_skill_results));
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // 길드 기본 정보 조회
        if (oguild_id) {
            const guild_basic_url = `https://open.api.nexon.com/maplestory/v1/guild/basic?oguild_id=${oguild_id}`;
            const guild_basic_response = await fetch(guild_basic_url, {
                headers: {
                    "x-nxopen-api-key": API_KEY,
                    "accept": "application/json",
                },
            });

            // 응답 확인
            if (!guild_basic_response.ok) {
                const errorMessage = `Failed to fetch character stats with status ${guild_basic_response.status}: ${guild_basic_response.statusText}`;
                console.error(errorMessage);
                return res.status(guild_basic_response.status).json({ error: errorMessage });
            }

            const guild_basic_json = await guild_basic_response.json();
            // 각 스킬의 "운 n 증가"에서 n 값을 추출하여 합산
            guild_basic_json.guild_skill.forEach((skill) => {
                const skill_effect = skill.skill_effect;
                if (skill_effect) {
                    // 정규 표현식을 사용해 "운 n" 또는 "올스탯 n"에서 n 값 추출
                    const match = skill_effect.match(/(?:운|올스탯)\s(\d+)/);
                    if (match) {
                        //console.log("skill_name : " + skill.skill_name);
                        //console.log("match : " + match);
                        const luckValue = parseInt(match[1], 10); // n 값을 숫자로 변환
                        totalLuck += luckValue; // 합산
                    }
                }
            });
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // HEXA 매트릭스 설정 HEXA 스탯 정보 조회
        const hexamatrix_stat_url = `https://open.api.nexon.com/maplestory/v1/character/hexamatrix-stat?ocid=${ocid}`;
        const hexamatrix_stat_response = await fetch(hexamatrix_stat_url, {
            headers: {
                "x-nxopen-api-key": API_KEY,
                "accept": "application/json",
            },
        });

        // 응답 확인
        if (!hexamatrix_stat_response.ok) {
            const errorMessage = `Failed to fetch character stats with status ${hexamatrix_stat_response.status}: ${hexamatrix_stat_response.statusText}`;
            console.error(errorMessage);
            return res.status(hexamatrix_stat_response.status).json({ error: errorMessage });
        }

        const hexamatrix_stat_json = await hexamatrix_stat_response.json();
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // 주스텟
        console.log("운(LUK)");
        console.log("기본 수치");

        // "AP 배분 LUK" 값 추출
        const ApDistributedLuk = stat_json.final_stat.find(
            (stat) => stat.stat_name === "AP 배분 LUK"
        );

        // 주스탯_기본 수치_기본
        console.log("기본 수치_기본 : " + ApDistributedLuk.stat_value);

        // 주스탯_기본 수치_스킬
        totalLuck = Math.floor(totalLuck);
        console.log("기본 수치_스킬 :", totalLuck);


        res.json({
            message: "Character data fetched successfully",
            ocid,
            stats: stat_json,
            hyper_stat: hyper_stat_json,
            propensity: propensity_json,
            ability: ability_json,
            item_equipment: item_equipment_json,
            cashitem_equipment: cashitem_equipment_json,
            symbol_equipment: symbol_equipment_json,
            set_effect: set_effect_json,
            pet_equipment: pet_equipment_json,
            hexamatrix_stat: hexamatrix_stat_json,
        });

        // "AP 배분 LUK" 값 추출
        const ApDistributedLuk = statsData.final_stat.find(
            (stat) => stat.stat_name === "AP 배분 LUK"
        );

        console.log("스탯");
        console.log("기본 수치_기본 : " + ApDistributedLuk);
        */

    } catch (error) {
        console.error('Error fetching character data:', error.message);
        res.status(500).json({ error: "An error occurred while fetching character data." });
    }
};