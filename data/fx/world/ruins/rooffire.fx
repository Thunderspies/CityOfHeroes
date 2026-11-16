#############################################################
## RoofFire.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	root
		BhvrOverride
			PositionOffset	0 1 0
		End
		Part	:RoofFire_flame_A.part
		Part	:RoofFire_flame_B.part
		Part	:RoofFireSpark.part
		Part	:RoofFire_Flame_Flat.part
		Part	:RoofFire_Flame_Flat_Center.part

		Sound buildingfire3_loop 50 50 .6

	End

	Event
		Type	Start
		At	root
		BhvrOverride
			PositionOffset	0 4.0 0
		End
		Part	:RoofFire_Smoke.part
		Part	:RoofFire_Smoke_Lingering.part
	End
End

#############################################################

End