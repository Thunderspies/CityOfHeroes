#############################################################
## Inferno.fx
#############################################################

FxInfo

Lifespan 5

#############################################################

Condition
	Chance 0.4
	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound 	CloseAirSupport_Gunfire_01 50 50 .4
		Sound 	CloseAirSupport_Gunfire_02 50 50 .4
		Sound 	CloseAirSupport_Gunfire_03 50 50 .4
		Sound 	CloseAirSupport_Gunfire_04 50 50 .4
		Sound 	CloseAirSupport_Gunfire_05 50 50 .4
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	StartPositOnly
		At	Origin
		Bhvr	behaviors/CameraShakeSoft.bhvr

		Part	:Flash_Glow.part
		Part	:Sparks_Impact.part
		Part	:Dust_Pillar.part
		Part	:Dirt_Splatter_Small.part
		POther	DustAnchor
		Lifespan	1
	End

	Event
		Type	StartPositOnly
		At	Origin
		BhvrOverride
			PositionOffset	0 0.5 0.0
		End
		Part	:Dust_RockBreak.part
		Lifespan	1
	End
End

#############################################################

End