TowerDefenseGame
===================
Practie & Demo

## Chapter 01
波與波的連段性待新增。

## Chapter 02
```C++
//地圖屬性要注意的地方，
Value props = this->tileMap->propertiesForGID(tileGid);
ValueMap map = props.asValueMap();
//[map.size() == 0] is not equal to [props.isNull()]
```
砲塔移動到超出地圖外的錯誤待修。