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
		SoundNoRepeat 3
		Sound Cat_FF_01 50 50 0.07
		Sound Cat_FF_02 50 50 0.07
		Sound Cat_FF_03 50 50 0.07
		Sound Cat_FF_04 50 50 0.07
		Sound Cat_FF_05 50 50 0.07
		Sound Cat_FF_06 50 50 0.07
		Sound Cat_FF_07 50 50 0.07
	End

End

Condition
	TriggerBits	ASPHALT
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 3
		Sound Cat_FF_01 50 50 0.07
		Sound Cat_FF_02 50 50 0.07
		Sound Cat_FF_03 50 50 0.07
		Sound Cat_FF_04 50 50 0.07
		Sound Cat_FF_05 50 50 0.07
		Sound Cat_FF_06 50 50 0.07
		Sound Cat_FF_07 50 50 0.07
	End

End

Condition
	TriggerBits	GRASS
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 3
		Sound Dog_FFGrass_01 50 50 0.05
		Sound Dog_FFGrass_02 50 50 0.05
		Sound Dog_FFGrass_03 50 50 0.05
		Sound Dog_FFGrass_04 50 50 0.05
		Sound Dog_FFGrass_05 50 50 0.05
		Sound Dog_FFGrass_06 50 50 0.05
	End
End

Condition
	TriggerBits	DIRT
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 3
		Sound Dog_FFDirt_01 50 50 0.07
		Sound Dog_FFDirt_02 50 50 0.07
		Sound Dog_FFDirt_03 50 50 0.07
		Sound Dog_FFDirt_04 50 50 0.07
		Sound Dog_FFDirt_05 50 50 0.07
		Sound Dog_FFDirt_06 50 50 0.07
	End
End

Condition
	TriggerBits	SAND
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 3
		Sound Dog_FFSand_01 50 50 0.05
		Sound Dog_FFSand_02 50 50 0.05
		Sound Dog_FFSand_03 50 50 0.05
		Sound Dog_FFSand_04 50 50 0.05
		Sound Dog_FFSand_05 50 50 0.05
		Sound Dog_FFSand_06 50 50 0.05
	End
End

Condition
	TriggerBits	GRAVEL
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 3
		Sound Dog_FFGravel_01 50 50 0.07
		Sound Dog_FFGravel_02 50 50 0.07
		Sound Dog_FFGravel_03 50 50 0.07
		Sound Dog_FFGravel_04 50 50 0.07
		Sound Dog_FFGravel_05 50 50 0.07
		Sound Dog_FFGravel_06 50 50 0.07
	End
End

Condition
	TriggerBits	PUDDLE
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 3
		Sound Dog_FFPuddle_01 50 50 0.1
		Sound Dog_FFPuddle_02 50 50 0.1
		Sound Dog_FFPuddle_03 50 50 0.1
		Sound Dog_FFPuddle_04 50 50 0.1
		Sound Dog_FFPuddle_05 50 50 0.1
		Sound Dog_FFPuddle_06 50 50 0.1
	End
End

Condition
	TriggerBits	INWATER
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 3
		Sound FF_Water_01 50 50 .5
		Sound FF_Water_02 50 50 .5
		Sound FF_Water_03 50 50 .5
		Sound FF_Water_04 50 50 .5
		Sound FF_Water_05 50 50 .5
	End
End

Condition
	TriggerBits	CATWALK
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 3
		Sound Dog_FFCatwalk_01 50 50 0.07
		Sound Dog_FFCatwalk_02 50 50 0.07
		Sound Dog_FFCatwalk_03 50 50 0.07
		Sound Dog_FFCatwalk_04 50 50 0.07
		Sound Dog_FFCatwalk_05 50 50 0.07
		Sound Dog_FFCatwalk_06 50 50 0.07
	End
End

Condition
	TriggerBits	METALPLATE
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 3
		Sound Dog_FFMetalPlate_01 50 50 0.07
		Sound Dog_FFMetalPlate_02 50 50 0.07
		Sound Dog_FFMetalPlate_03 50 50 0.07
		Sound Dog_FFMetalPlate_04 50 50 0.07
		Sound Dog_FFMetalPlate_05 50 50 0.07
		Sound Dog_FFMetalPlate_06 50 50 0.07
	End
End

Condition
	TriggerBits	METAL
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 3
		Sound Dog_FFMetal_01 50 50 0.05
		Sound Dog_FFMetal_02 50 50 0.05
		Sound Dog_FFMetal_03 50 50 0.05
		Sound Dog_FFMetal_04 50 50 0.05
		Sound Dog_FFMetal_05 50 50 0.05
		Sound Dog_FFMetal_06 50 50 0.05
	End
End

Condition
	TriggerBits	WOOD
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 3
		Sound Dog_FFWood_01 50 50 0.1
		Sound Dog_FFWood_02 50 50 0.1
		Sound Dog_FFWood_03 50 50 0.1
		Sound Dog_FFWood_04 50 50 0.1
		Sound Dog_FFWood_05 50 50 0.1
		Sound Dog_FFWood_06 50 50 0.1
	End
End

Condition
	TriggerBits	WOODHOLLOW
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 3
		Sound Dog_FFWood_01 50 50 0.1
		Sound Dog_FFWood_02 50 50 0.1
		Sound Dog_FFWood_03 50 50 0.1
		Sound Dog_FFWood_04 50 50 0.1
		Sound Dog_FFWood_05 50 50 0.1
		Sound Dog_FFWood_06 50 50 0.1
	End
End

Condition
	TriggerBits	CARPET
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 3
		Sound Dog_FFSoft_01 50 50 0.07
		Sound Dog_FFSoft_02 50 50 0.07
		Sound Dog_FFSoft_03 50 50 0.07
		Sound Dog_FFSoft_04 50 50 0.07
		Sound Dog_FFSoft_05 50 50 0.07
		Sound Dog_FFSoft_06 50 50 0.07
		Sound Dog_FFSoft_07 50 50 0.07
	End

End

Condition
	TriggerBits	TILE
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 3
		Sound Cat_FF_01 50 50 0.07
		Sound Cat_FF_02 50 50 0.07
		Sound Cat_FF_03 50 50 0.07
		Sound Cat_FF_04 50 50 0.07
		Sound Cat_FF_05 50 50 0.07
		Sound Cat_FF_06 50 50 0.07
		Sound Cat_FF_07 50 50 0.07
	End

End

Condition
	TriggerBits	SNOW
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 3
		Sound Dog_FFSnow_01 50 50 0.05
		Sound Dog_FFSnow_02 50 50 0.05
		Sound Dog_FFSnow_03 50 50 0.05
		Sound Dog_FFSnow_04 50 50 0.05
		Sound Dog_FFSnow_05 50 50 0.05
		Sound Dog_FFSnow_06 50 50 0.05
	End
End
