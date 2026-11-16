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
		Sound PPD_FF_02 80 80 .3
		Sound PPD_FF_04 80 80 .3
		Sound PPD_FF_06 80 80 .3
		Sound PPD_FF_08 80 80 .3
		Sound PPD_FF_10 80 80 .3
	End					
				
End						
						
Condition						
	TriggerBits	ASPHALT				
	Event					
		Type	Local			
		At	Origin		
		SoundNoRepeat 2	
		Sound PPD_FF_02 80 80 .3
		Sound PPD_FF_04 80 80 .3
		Sound PPD_FF_06 80 80 .3
		Sound PPD_FF_08 80 80 .3
		Sound PPD_FF_10 80 80 .3
	End					
			
End						
						
Condition						
	TriggerBits	GRASS				
	Event					
		Type	Local			
		At	Origin		
		SoundNoRepeat 2	
		Sound FF_Grass_02 80 80 0.2
		Sound FF_Grass_04 80 80 0.2
		Sound FF_Grass_06 80 80 0.2
		Sound FF_Grass_08 80 80 0.2
		Sound FF_Grass_10 80 80 0.2
	End											
	Event					
		Type	Local			
		At 		Origin		
		SoundNoRepeat 2
		Sound PPD_FF_02 80 80 .3
		Sound PPD_FF_04 80 80 .3
		Sound PPD_FF_06 80 80 .3
		Sound PPD_FF_08 80 80 .3
		Sound PPD_FF_10 80 80 .3
	End					
End	
						
Condition						
	TriggerBits	DIRT				
	Event					
		Type	Local			
		At	Origin		
		SoundNoRepeat 2	
		Sound FF_Dirt_02 80 80 0.2
		Sound FF_Dirt_04 80 80 0.2
		Sound FF_Dirt_06 80 80 0.2
		Sound FF_Dirt_08 80 80 0.2
		Sound FF_Dirt_10 80 80 0.2
	End												
	Event					
		Type	Local			
		At 		Origin		
		SoundNoRepeat 2
		Sound PPD_FF_02 80 80 .3
		Sound PPD_FF_04 80 80 .3
		Sound PPD_FF_06 80 80 .3
		Sound PPD_FF_08 80 80 .3
		Sound PPD_FF_10 80 80 .3
	End					
End	
						
Condition						
	TriggerBits	SAND				
	Event					
		Type	Local			
		At	Origin		
		SoundNoRepeat 2	
		Sound FF_Sand_02 80 80 0.2
		Sound FF_Sand_04 80 80 0.2
		Sound FF_Sand_06 80 80 0.2
		Sound FF_Sand_08 80 80 0.2
		Sound FF_Sand_10 80 80 0.2
	End								
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound PPD_FF_02 80 80 .3
		Sound PPD_FF_04 80 80 .3
		Sound PPD_FF_06 80 80 .3
		Sound PPD_FF_08 80 80 .3
		Sound PPD_FF_10 80 80 .3
	End					
End					
						
Condition						
	TriggerBits	GRAVEL				
	Event					
		Type	Local			
		At	Origin		
		SoundNoRepeat 2	
		Sound FF_Gravel_02 80 80 0.5
		Sound FF_Gravel_04 80 80 0.5
		Sound FF_Gravel_06 80 80 0.5
		Sound FF_Gravel_08 80 80 0.5
		Sound FF_Gravel_10 80 80 0.5
	End												
	Event					
		Type	Local			
		At 		Origin		
		SoundNoRepeat 2
		Sound PPD_FF_02 80 80 .3
		Sound PPD_FF_04 80 80 .3
		Sound PPD_FF_06 80 80 .3
		Sound PPD_FF_08 80 80 .3
		Sound PPD_FF_10 80 80 .3
	End					
End	
						
Condition						
	TriggerBits	PUDDLE				
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound FF_Puddle_02 80 80 0.4
		Sound FF_Puddle_04 80 80 0.4
		Sound FF_Puddle_06 80 80 0.4
		Sound FF_Puddle_08 80 80 0.4
		Sound FF_Puddle_10 80 80 0.4
	End											
	Event					
		Type	Local			
		At 		Origin		
		SoundNoRepeat 2
		Sound PPD_FF_02 80 80 .3
		Sound PPD_FF_04 80 80 .3
		Sound PPD_FF_06 80 80 .3
		Sound PPD_FF_08 80 80 .3
		Sound PPD_FF_10 80 80 .3
	End					
End					
						
Condition						
	TriggerBits	INWATER				
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound FF_Water_02 80 80 0.5
		Sound FF_Water_04 80 80 0.5
		Sound FF_Water_06 80 80 0.5
		Sound FF_Water_08 80 80 0.5
		Sound FF_Water_10 80 80 0.5
	End					
End						
						
Condition						
	TriggerBits	CATWALK				
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound FF_Catwalk_02 80 80 0.2
		Sound FF_Catwalk_04 80 80 0.2
		Sound FF_Catwalk_06 80 80 0.2
		Sound FF_Catwalk_08 80 80 0.2
		Sound FF_Catwalk_10 80 80 0.2
	End								
	Event					
		Type	Local			
		At 		Origin		
		SoundNoRepeat 2
		Sound PPD_FF_02 80 80 .3
		Sound PPD_FF_04 80 80 .3
		Sound PPD_FF_06 80 80 .3
		Sound PPD_FF_08 80 80 .3
		Sound PPD_FF_10 80 80 .3
	End					
End	
						
Condition						
	TriggerBits	METALPLATE				
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound FF_MetalPlate_02 80 80 0.3
		Sound FF_MetalPlate_04 80 80 0.3
		Sound FF_MetalPlate_06 80 80 0.3
		Sound FF_MetalPlate_08 80 80 0.3
		Sound FF_MetalPlate_10 80 80 0.3
	End													
	Event					
		Type	Local			
		At 		Origin		
		SoundNoRepeat 2
		Sound PPD_FF_02 80 80 .3
		Sound PPD_FF_04 80 80 .3
		Sound PPD_FF_06 80 80 .3
		Sound PPD_FF_08 80 80 .3
		Sound PPD_FF_10 80 80 .3
	End					
End	
						
Condition						
	TriggerBits	METAL				
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound FF_Metal_02 80 80 0.2
		Sound FF_Metal_04 80 80 0.2
		Sound FF_Metal_06 80 80 0.2
		Sound FF_Metal_08 80 80 0.2
		Sound FF_Metal_10 80 80 0.2
	End												
	Event					
		Type	Local			
		At 		Origin		
		SoundNoRepeat 2
		Sound PPD_FF_02 80 80 .3
		Sound PPD_FF_04 80 80 .3
		Sound PPD_FF_06 80 80 .3
		Sound PPD_FF_08 80 80 .3
		Sound PPD_FF_10 80 80 .3
	End					
End						
						
Condition						
	TriggerBits	WOOD				
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound PPD_FF_02 80 80 .3
		Sound PPD_FF_04 80 80 .3
		Sound PPD_FF_06 80 80 .3
		Sound PPD_FF_08 80 80 .3
		Sound PPD_FF_10 80 80 .3
	End					
End						
						
Condition						
	TriggerBits	WOODHOLLOW				
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound FF_WoodHollow_02 80 80 0.4
		Sound FF_WoodHollow_04 80 80 0.4
		Sound FF_WoodHollow_06 80 80 0.4
		Sound FF_WoodHollow_08 80 80 0.4
		Sound FF_WoodHollow_10 80 80 0.4
	End								
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound PPD_FF_02 80 80 .3
		Sound PPD_FF_04 80 80 .3
		Sound PPD_FF_06 80 80 .3
		Sound PPD_FF_08 80 80 .3
		Sound PPD_FF_10 80 80 .3
	End					
End				

					
						
Condition						
	TriggerBits	CARPET				
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound PPD_FF_02 80 80 .3
		Sound PPD_FF_04 80 80 .3
		Sound PPD_FF_06 80 80 .3
		Sound PPD_FF_08 80 80 .3
		Sound PPD_FF_10 80 80 .3
	End					
				
End						
						
Condition						
	TriggerBits	TILE				
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound PPD_FF_02 80 80 .3
		Sound PPD_FF_04 80 80 .3
		Sound PPD_FF_06 80 80 .3
		Sound PPD_FF_08 80 80 .3
		Sound PPD_FF_10 80 80 .3
	End					
End						
						
Condition						
	TriggerBits	SNOW				
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound FF_Snow_02 80 80 0.2
		Sound FF_Snow_04 80 80 0.2
		Sound FF_Snow_06 80 80 0.2
		Sound FF_Snow_08 80 80 0.2
		Sound FF_Snow_10 80 80 0.2
	End					
	Event					
		Type	Local			
		At	Origin			
		SoundNoRepeat 2
		Sound PPD_FF_02 80 80 .3
		Sound PPD_FF_04 80 80 .3
		Sound PPD_FF_06 80 80 .3
		Sound PPD_FF_08 80 80 .3
		Sound PPD_FF_10 80 80 .3
	End					
End	