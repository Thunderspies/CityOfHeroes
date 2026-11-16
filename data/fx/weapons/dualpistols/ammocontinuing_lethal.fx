#############################################################
## AmmoContinuing_Lethal.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	MuzzleFlashOffset_Left
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset		0.75 0 0.375
		End
		ChildFX	WEAPONS\DualPistols\Child_VaporTrail.fx
	End

	Event
		EName	MuzzleFlashOffset_Right
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset		-0.75 0 0.375
		End
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	MuzzleFlashOffset_Left
		Part1	POWERS\FireControl\FireHandz.part
		LifeSpan 60
	End

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		Part1	POWERS\FireControl\FireHandz.part
		LifeSpan 60
	End
End

#############################################################

End