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
		#Part1	WEAPONS\DualPistols\Continuing_FireTendrils01.part
	End

	Event
		EName	MuzzleFlashOffset_Right
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset		-0.75 0 0.375
		End
		#Part1	WEAPONS\DualPistols\Continuing_FireTendrils01.part
	End
End

#############################################################

End