#########################################################			
## 	Huge Left Foot		
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
		Sound Ghoul_FF_01 50 50 .1
		Sound Ghoul_FF_03 50 50 .1
		Sound Ghoul_FF_05 50 50 .1
		Sound Ghoul_FF_07 50 50 .1
		Sound Ghoul_FF_09 50 50 .1
	End					
				
End						
						
Condition						
	TriggerBits	ASPHALT				
	Event					
		Type	Local			
		At	Origin		
		SoundNoRepeat 2	
		Sound Ghoul_FF_01 50 50 .1
		Sound Ghoul_FF_03 50 50 .1
		Sound Ghoul_FF_05 50 50 .1
		Sound Ghoul_FF_07 50 50 .1
		Sound Ghoul_FF_09 50 50 .1
	End					
			
End						
						
Condition						
	TriggerBits	GRASS				
	Event					
		Type	Local			
		At	Origin		
		SoundNoRepeat 2	
		Sound FF_Grass_01 50 50 0.2
		Sound FF_Grass_03 50 50 0.2
		Sound FF_Grass_05 50 50 0.2
		Sound FF_Grass_07 50 50 0.2
		Sound FF_Grass_09 50 50 0.2
	End
	Event		
		Type	Local
		At	Origin	
		SoundNoRepeat 2		
		Sound Ghoul_FF_01 50 50 .2
		Sound Ghoul_FF_03 50 50 .2
		Sound Ghoul_FF_05 50 50 .2
		Sound Ghoul_FF_07 50 50 .2
		Sound Ghoul_FF_09 50 50 .2
	End			
			
End						
						
Condition						
	TriggerBits	DIRT				
	Event					
		Type	Local			
		At	Origin		
		SoundNoRepeat 2	
		Sound FF_Dirt_01 50 50 0.2
		Sound FF_Dirt_03 50 50 0.2
		Sound FF_Dirt_05 50 50 0.2
		Sound FF_Dirt_07 50 50 0.2
		Sound FF_Dirt_09 50 50 0.2
	End					
	Event		
		Type	Local
		At	Origin	
		SoundNoRepeat 2		
		Sound Ghoul_FF_01 50 50 .2
		Sound Ghoul_FF_03 50 50 .2
		Sound Ghoul_FF_05 50 50 .2
		Sound Ghoul_FF_07 50 50 .2
		Sound Ghoul_FF_09 50 50 .2
	End		
End						
						
Condition						
	TriggerBits	SAND				
	Event					
		Type	Local			
		At	Origin		
		SoundNoRepeat 2	
		Sound FF_Sand_01 50 50 0.2
		Sound FF_Sand_03 50 50 0.2
		Sound FF_Sand_05 50 50 0.2
		Sound FF_Sand_07 50 50 0.2
		Sound FF_Sand_09 50 50 0.2
	End					
	Event		
		Type	Local
		At	Origin	
		SoundNoRepeat 2		
		Sound Ghoul_FF_01 50 50 .2
		Sound Ghoul_FF_03 50 50 .2
		Sound Ghoul_FF_05 50 50 .2
		Sound Ghoul_FF_07 50 50 .2
		Sound Ghoul_FF_09 50 50 .2
	End		
End						
						
Condition						
	TriggerBits	GRAVEL				
	Event					
		Type	Local			
		At	Origin		
		SoundNoRepeat 2	
		Sound FF_Gravel_01 50 50 0.5
		Sound FF_Gravel_03 50 50 0.5
		Sound FF_Gravel_05 50 50 0.5
		Sound FF_Gravel_07 50 50 0.5
		Sound FF_Gravel_09 50 50 0.5
	End					
	Event		
		Type	Local
		At	Origin	
		SoundNoRepeat 2		
		Sound Ghoul_FF_01 50 50 .2
		Sound Ghoul_FF_03 50 50 .2
		Sound Ghoul_FF_05 50 50 .2
		Sound Ghoul_FF_07 50 50 .2
		Sound Ghoul_FF_09 50 50 .2
	End		
End						
						
Condition						
	TriggerBits	PUDDLE				
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound FF_Puddle_01 50 50 0.4
		Sound FF_Puddle_03 50 50 0.4
		Sound FF_Puddle_05 50 50 0.4
		Sound FF_Puddle_07 50 50 0.4
		Sound FF_Puddle_09 50 50 0.4
	End					
	Event		
		Type	Local
		At	Origin	
		SoundNoRepeat 2		
		Sound Ghoul_FF_01 50 50 .2
		Sound Ghoul_FF_03 50 50 .2
		Sound Ghoul_FF_05 50 50 .2
		Sound Ghoul_FF_07 50 50 .2
		Sound Ghoul_FF_09 50 50 .2
	End		
End										
						
Condition						
	TriggerBits	INWATER				
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound FF_Water_01 50 50 0.5
		Sound FF_Water_03 50 50 0.5
		Sound FF_Water_05 50 50 0.5
		Sound FF_Water_07 50 50 0.5
		Sound FF_Water_09 50 50 0.5
	End					
End						
						
Condition						
	TriggerBits	CATWALK				
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound FF_Catwalk_01 50 50 0.4
		Sound FF_Catwalk_03 50 50 0.4
		Sound FF_Catwalk_05 50 50 0.4
		Sound FF_Catwalk_07 50 50 0.4
		Sound FF_Catwalk_09 50 50 0.4
	End					
	Event		
		Type	Local
		At	Origin	
		SoundNoRepeat 2		
		Sound Ghoul_FF_01 50 50 .2
		Sound Ghoul_FF_03 50 50 .2
		Sound Ghoul_FF_05 50 50 .2
		Sound Ghoul_FF_07 50 50 .2
		Sound Ghoul_FF_09 50 50 .2
	End		
End						
						
Condition						
	TriggerBits	METALPLATE				
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound FF_MetalPlate_01 50 50 0.7
		Sound FF_MetalPlate_03 50 50 0.7
		Sound FF_MetalPlate_05 50 50 0.7
		Sound FF_MetalPlate_07 50 50 0.7
		Sound FF_MetalPlate_09 50 50 0.7
	End					
	Event		
		Type	Local
		At	Origin	
		SoundNoRepeat 2		
		Sound Ghoul_FF_01 50 50 .2
		Sound Ghoul_FF_03 50 50 .2
		Sound Ghoul_FF_05 50 50 .2
		Sound Ghoul_FF_07 50 50 .2
		Sound Ghoul_FF_09 50 50 .2
	End		
End						
						
Condition						
	TriggerBits	METAL				
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound FF_Metal_01 50 50 0.5
		Sound FF_Metal_03 50 50 0.5
		Sound FF_Metal_05 50 50 0.5
		Sound FF_Metal_07 50 50 0.5
		Sound FF_Metal_09 50 50 0.5
	End					
	Event		
		Type	Local
		At	Origin	
		SoundNoRepeat 2		
		Sound Ghoul_FF_01 50 50 .2
		Sound Ghoul_FF_03 50 50 .2
		Sound Ghoul_FF_05 50 50 .2
		Sound Ghoul_FF_07 50 50 .2
		Sound Ghoul_FF_09 50 50 .2
	End		
End					
						
Condition						
	TriggerBits	WOOD				
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound Ghoul_FF_01 50 50 .2
		Sound Ghoul_FF_03 50 50 .2
		Sound Ghoul_FF_05 50 50 .2
		Sound Ghoul_FF_07 50 50 .2
		Sound Ghoul_FF_09 50 50 .2
	End					
End						
						
Condition						
	TriggerBits	WOODHOLLOW				
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound FF_WoodHollow_01 50 50 0.4
		Sound FF_WoodHollow_03 50 50 0.4
		Sound FF_WoodHollow_05 50 50 0.4
		Sound FF_WoodHollow_07 50 50 0.4
		Sound FF_WoodHollow_09 50 50 0.4
	End					
	Event		
		Type	Local
		At	Origin	
		SoundNoRepeat 2		
		Sound Ghoul_FF_01 50 50 .2
		Sound Ghoul_FF_03 50 50 .2
		Sound Ghoul_FF_05 50 50 .2
		Sound Ghoul_FF_07 50 50 .2
		Sound Ghoul_FF_09 50 50 .2
	End		
End						
						
Condition						
	TriggerBits	CARPET				
	Event		
		Type	Local
		At	Origin	
		SoundNoRepeat 2		
		Sound Ghoul_FF_01 50 50 .2
		Sound Ghoul_FF_03 50 50 .2
		Sound Ghoul_FF_05 50 50 .2
		Sound Ghoul_FF_07 50 50 .2
		Sound Ghoul_FF_09 50 50 .2
	End						
				
End						
						
Condition						
	TriggerBits	TILE				
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound Ghoul_FF_01 50 50 .2
		Sound Ghoul_FF_03 50 50 .2
		Sound Ghoul_FF_05 50 50 .2
		Sound Ghoul_FF_07 50 50 .2
		Sound Ghoul_FF_09 50 50 .2
	End					
				
End						
						
Condition						
	TriggerBits	SNOW				
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound FF_Snow_01 50 50 0.2
		Sound FF_Snow_03 50 50 0.2
		Sound FF_Snow_05 50 50 0.2
		Sound FF_Snow_07 50 50 0.2
		Sound FF_Snow_09 50 50 0.2
	End					
	Event		
		Type	Local
		At	Origin	
		SoundNoRepeat 2		
		Sound Ghoul_FF_01 50 50 .2
		Sound Ghoul_FF_03 50 50 .2
		Sound Ghoul_FF_05 50 50 .2
		Sound Ghoul_FF_07 50 50 .2
		Sound Ghoul_FF_09 50 50 .2
	End		
End						
