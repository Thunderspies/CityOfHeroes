#############################################################
## Test.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	TriggerBits
	TriggerBits	MALE
	DoMany		1

	Event
		EName	HolsteredPistols
		Type	Local
		At	Root
		Flags	OneAtATime
		Until	DUALHAND
		ChildFX	WEAPONS\Custom_Pistols\Maelstrom\Maelstrom_HolsteredPistols.fx
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Pistol_Right
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_Pistols\Maelstrom\Maelstrom_HoloSights_Fade.bhvr
		Anim	CustomWeapon_PistolRight_Maelstrom
	End

	Event
		EName	Pistol_Left
		Type	Local
		At	WepL
		Bhvr	WEAPONS\Custom_Pistols\Maelstrom\Maelstrom_HoloSights_Fade.bhvr
		BhvrOverride
			PYRRotate		0 0 180
		End
		Anim	CustomWeapon_PistolLeft_Maelstrom
	End
End

#############################################################

Condition
	On 	TriggerBits
	TriggerBits 	DUALHAND
	DoMany		1

	Event
		EName	HolographicSights
		Type	Local
		At	WepR
		While	DUALHAND
		Flags	OneAtATime
		ChildFX	WEAPONS\Custom_Pistols\Maelstrom\Maelstrom_HoloSights.fx
	End
End

#############################################################

End