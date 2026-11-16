#############################################################
## RoofFire_TEST.fx
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
			PositionOFfset	0 1 0
		End
		Part	:RoofFire_flame_A.part
		Part	:RoofFire_flame_B.part
		Part	:RoofFireSpark.part
		Part	:RoofFire_Flame_Flat.part
		Part	:RoofFire_Flame_Flat_Center.part

		Sound buildingfire3_loop 180 180 .89

	End
End

#############################################################

End