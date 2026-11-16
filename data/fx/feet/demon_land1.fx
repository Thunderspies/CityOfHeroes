#########################################################			
## 	Huge Stops		
#########################################################			

FxInfo			

Input			
	InpName Origin		
End			

LifeSpan 1			

Condition			
	TriggerBits	CONCRETE	
	Event		
		Type	Local
		At	Origin	
		SoundNoRepeat 2		
		Sound FF_Sidewalk_01 80 80 0.6
		Sound FF_Sidewalk_03 80 80 0.6
		Sound FF_Sidewalk_05 80 80 0.6
		Sound FF_Sidewalk_07 80 80 0.6
		Sound FF_Sidewalk_09 80 80 0.6
	End					
	Event					
		Type	Local			
		At	origin	
		Sound FF_DemonSweet_01 80 80 0.3
		Sound FF_DemonSweet_02 80 80 0.3
	End					
End						
						
Condition						
	TriggerBits	ASPHALT				
	Event					
		Type	Local			
		At	Origin
		SoundNoRepeat 2
		Sound FF_Street_01 80 80 0.5
		Sound FF_Street_03 80 80 0.5
		Sound FF_Street_05 80 80 0.5
		Sound FF_Street_07 80 80 0.5
		Sound FF_Street_09 80 80 0.5
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 80 80 0.3
		Sound FF_DemonSweet_02 80 80 0.3
	End					
End						
						
Condition						
	TriggerBits	GRASS				
	Event					
		Type	Local			
		At	Origin	
		SoundNoRepeat 2
		Sound FF_Grass_01 80 80 0.2
		Sound FF_Grass_03 80 80 0.2
		Sound FF_Grass_05 80 80 0.2
		Sound FF_Grass_07 80 80 0.2
		Sound FF_Grass_09 80 80 0.2
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 80 80 0.2
		Sound FF_DemonSweet_02 80 80 0.2
	End					
End						
						
Condition						
	TriggerBits	DIRT				
	Event					
		Type	Local			
		At	Origin	
		SoundNoRepeat 2
		Sound FF_Dirt_01 80 80 0.3
		Sound FF_Dirt_03 80 80 0.3
		Sound FF_Dirt_05 80 80 0.3
		Sound FF_Dirt_07 80 80 0.3
		Sound FF_Dirt_09 80 80 0.3
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 80 80 0.3
		Sound FF_DemonSweet_02 80 80 0.3
	End					
End						
						
Condition						
	TriggerBits	SAND				
	Event					
		Type	Local			
		At	Origin	
		SoundNoRepeat 2
		Sound FF_Sand_01 80 80 0.3
		Sound FF_Sand_03 80 80 0.3
		Sound FF_Sand_05 80 80 0.3
		Sound FF_Sand_07 80 80 0.3
		Sound FF_Sand_09 80 80 0.3
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 80 80 0.3
		Sound FF_DemonSweet_02 80 80 0.3
	End					
End						
						
Condition						
	TriggerBits	GRAVEL				
	Event					
		Type	Local			
		At	Origin	
		SoundNoRepeat 2
		Sound FF_Gravel_01 80 80 0.5
		Sound FF_Gravel_03 80 80 0.5
		Sound FF_Gravel_05 80 80 0.5
		Sound FF_Gravel_07 80 80 0.5
		Sound FF_Gravel_09 80 80 0.5
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 80 80 0.3
		Sound FF_DemonSweet_02 80 80 0.3
	End					
End						
						
Condition						
	TriggerBits	PUDDLE				
	Event					
		Type	Local			
		At	Origin
		SoundNoRepeat 2
		Sound FF_Puddle_01 80 80 0.4
		Sound FF_Puddle_03 80 80 0.4
		Sound FF_Puddle_05 80 80 0.4
		Sound FF_Puddle_07 80 80 0.4
		Sound FF_Puddle_09 80 80 0.4
	End					
End						
Condition						
	TriggerBits PUDDLE					
	Chance	0.1				
	Event					
		Type	Local			
		At 		Origin		
		SoundNoRepeat 2
		Sound WaterExit1 80 80 0.2
		Sound WaterExit2 80 80 0.2
		Sound WaterExit3 80 80 0.2
		Sound WaterExit4 80 80 0.2
		Sound WaterExit5 80 80 0.2
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 80 80 0.3
		Sound FF_DemonSweet_02 80 80 0.3
	End					
End						
						
Condition						
	TriggerBits	INWATER				
	Event					
		Type	Local			
		At	Origin	
		SoundNoRepeat 2
		Sound FF_Water_01 80 80 0.5
		Sound FF_Water_03 80 80 0.5
		Sound FF_Water_05 80 80 0.5
		Sound FF_Water_07 80 80 0.5
		Sound FF_Water_09 80 80 0.5
	End					
End						
						
Condition						
	TriggerBits	CATWALK				
	Event					
		Type	Local			
		At	Origin		
		SoundNoRepeat 2
		Sound FF_Catwalk_01 80 80 0.4
		Sound FF_Catwalk_03 80 80 0.4
		Sound FF_Catwalk_05 80 80 0.4
		Sound FF_Catwalk_07 80 80 0.4
		Sound FF_Catwalk_09 80 80 0.4
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 80 80 0.4
		Sound FF_DemonSweet_02 80 80 0.4
	End					
End						
						
Condition						
	TriggerBits	METALPLATE				
	Event					
		Type	Local			
		At	Origin		
		SoundNoRepeat 2
		Sound FF_MetalPlate_01 80 80 0.7
		Sound FF_MetalPlate_03 80 80 0.7
		Sound FF_MetalPlate_05 80 80 0.7
		Sound FF_MetalPlate_07 80 80 0.7
		Sound FF_MetalPlate_09 80 80 0.7
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 80 80 0.4
		Sound FF_DemonSweet_02 80 80 0.4
	End					
End						
						
Condition						
	TriggerBits	METAL				
	Event					
		Type	Local			
		At	Origin
		SoundNoRepeat 2
		Sound FF_Metal_01 80 80 0.5
		Sound FF_Metal_03 80 80 0.5
		Sound FF_Metal_05 80 80 0.5
		Sound FF_Metal_07 80 80 0.5
		Sound FF_Metal_09 80 80 0.5
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 80 80 0.4
		Sound FF_DemonSweet_02 80 80 0.4
	End					
End						
						
Condition						
	TriggerBits	WOOD				
	Event					
		Type	Local			
		At	Origin	
		SoundNoRepeat 2
		Sound FF_Wood_01 80 80 0.9
		Sound FF_Wood_03 80 80 0.9
		Sound FF_Wood_05 80 80 0.9
		Sound FF_Wood_07 80 80 0.9
		Sound FF_Wood_09 80 80 0.9
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 80 80 0.4
		Sound FF_DemonSweet_02 80 80 0.4
	End					
End						
						
Condition						
	TriggerBits	WOODHOLLOW				
	Event					
		Type	Local			
		At	Origin	
		SoundNoRepeat 2
		Sound FF_WoodHollow_01 80 80 0.9
		Sound FF_WoodHollow_03 80 80 0.9
		Sound FF_WoodHollow_05 80 80 0.9
		Sound FF_WoodHollow_07 80 80 0.9
		Sound FF_WoodHollow_09 80 80 0.9
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 80 80 0.4
		Sound FF_DemonSweet_02 80 80 0.4
	End					
End						
						
Condition						
	TriggerBits	CARPET				
	Event					
		Type	Local			
		At	Origin
		SoundNoRepeat 2
		Sound FF_Carpet_01 80 80 0.5
		Sound FF_Carpet_03 80 80 0.5
		Sound FF_Carpet_05 80 80 0.5
		Sound FF_Carpet_07 80 80 0.5
		Sound FF_Carpet_09 80 80 0.5
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 80 80 0.3
		Sound FF_DemonSweet_02 80 80 0.3
	End					
End						
						
Condition						
	TriggerBits	TILE				
	Event					
		Type	Local			
		At	Origin	
		SoundNoRepeat 2
		Sound FF_Tile_01 80 80 0.5
		Sound FF_Tile_03 80 80 0.5
		Sound FF_Tile_05 80 80 0.5
		Sound FF_Tile_07 80 80 0.5
		Sound FF_Tile_09 80 80 0.5
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 80 80 0.3
		Sound FF_DemonSweet_02 80 80 0.3
	End					
End						
						
Condition						
	TriggerBits	SNOW				
	Event					
		Type	Local			
		At	Origin
		SoundNoRepeat 2
		Sound FF_Snow_01 80 80 0.3
		Sound FF_Snow_03 80 80 0.3
		Sound FF_Snow_05 80 80 0.3
		Sound FF_Snow_07 80 80 0.3
		Sound FF_Snow_09 80 80 0.3
	End					
	Event					
		Type	Local			
		At	origin			
		Sound FF_DemonSweet_01 80 80 0.3
		Sound FF_DemonSweet_02 80 80 0.3
	End
End	
