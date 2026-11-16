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

		Part	:Flame_Small_Detached.part
		Part	:Flame_Small.part
		Part	:Flame_Small_Root.part
		Part	:RoofFireSpark.part

		Sound buildingfire3_loop 180 180 .89

	End

	Event
		Type	Start
		At	root
		BhvrOverride
			PositionOffset	0 2.0 0
		End
	#	Part	:RoofFire_Smoke.part
		Part	:Smoke_Small.part
	End
End

#############################################################

End