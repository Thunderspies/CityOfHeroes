#############################################################
## MediumFireSlow.fx
#############################################################

FxInfo
LifeSpan 0
Input
	InpName	Origin
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Fire
		Type 	Start
		At	Origin
		BhvrOverride
			PyrRotate	-90 -90 0
		End
		Part	:BuildingFire_Fire_Alpha.part
		Part	:BuildingFire_FireSmoke.part
	End

	Event
		EName 	SmokeColumn
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 15 0
			PyrRotate	-90 -90 0
		End
		Part	:BuildingFire_SmokePuff.part
		Part	:BuildingFire_Smoke.part
	End

End



#############################################################

End