

# To do:

Now:
- Spawners

- Create Basic Scenes
- Create Menus


----


Soon:
- Basic Enemies
- Biomes
- Basic Attack


----


Later:
- Database
- Advanced Level Creator
- Other Levels
- Attack Speed
- Boost Panels
- Toggleable Blocks
- Expand Moveset
- Equipment
- More Enemies
- Enemy AI
- Textures


# Memo:

```


Spawner needs:
- 'When' to spawn (member)
- 'Where' to spawn (member)
- 'What' to spawn (member)
- Spawning parameters (derived)
- Context to parameterize (argument)
- Method for handling and checking context (Game member)

'When':
- Public method
- Time period
- Buttons
- Scene load
- Within frame

Context:
- Scenes names
- Profiles names

'What':
- Any Entities (through constructor)




error: binding reference of type 

‘ nlohmann::json_abi_v3_12_0::json&’                                 {aka ‘ nlohmann::json_abi_v3_12_0::basic_json<>&’} to 
‘ const nlohmann::json_abi_v3_12_0::basic_json<>::value_type’        {aka ‘ const nlohmann::json_abi_v3_12_0::basic_json<>’} discards qualifiers


  122 |                 Scene scene = Scene().load(JsonFwd{j["data"]});
      |                                                             ^



      

```









