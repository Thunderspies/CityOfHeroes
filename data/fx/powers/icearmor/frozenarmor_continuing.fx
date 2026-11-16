#############################################################
## Ice Armor - Frozen Armor Continuing
#############################################################

FxInfo

Flags    InheritAlpha

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At 	origin
		Sound IceEmerge3 80 80 .95
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At 	origin
		bhvr	behaviors/soundFade3.bhvr
		Sound IceArmor_loop 80 80 .5
		Flags	PowerLoopingSound
		Lifespan	250
	End
End

## ICE ARMOR ACTIVATION FX ###########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName   Wind
		Type	start
		At	Root
		Bhvr 	powers/coldcontrol/IceArmorWind.bhvr
		geom	FX_TmpCtrlWisp01
		part1	:IceMistUpwards.part
		part2	:SnowMistUpwards.part
		Lifespan	26
	End
End

Condition
	On 	Time
	Time 	15
	Event
		EName   WindFast
		Type	start
		At	Root
		Bhvr 	powers/coldcontrol/IceArmorWindFast.bhvr
		geom	FX_TmpCtrlWisp01
		Lifespan	24
	End
End

Condition
	On 	Time
	Time 	20
	Event
		EName   WindFirst
		Type	start
		At	Root
		Bhvr 	powers/coldcontrol/IceArmorWindFirst.bhvr
		geom	FX_TmpCtrlWisp01
		Lifespan	24
	End
End

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	LLegL
		part2	:FrozenArmor_IceChunks_Medium.part
		pOther	FootL
	End

	Event
		Type	Local
		At	LLegR
		part2	:FrozenArmor_IceChunks_Medium.part
		pOther	FootR
	End

#	Event
#		Type	Local
#		At	FootL
#		part2	:FrozenArmor_IceChunks_Small.part
#	End
#
#	Event
#		Type	Local
#		At	FootR
#		part2	:FrozenArmor_IceChunks_Small.part
#	End
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	ULegL
		part2	:FrozenArmor_IceChunks_Medium.part
		pOther	LLegL
	End

	Event
		Type	Local
		At	ULegR
		part2	:FrozenArmor_IceChunks_Medium.part
		pOther	LLegR
	End
End

#############################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	UArmL
		part2	:FrozenArmor_IceChunks_Small.part
		pOther	LArmL
	End

	Event
		Type	Local
		At	UArmR
		part2	:FrozenArmor_IceChunks_Small.part
		pOther	LArmR
	End
End

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		Type	Local
		At	LArmR
		part2	:FrozenArmor_IceChunks_Small.part
		pOther	HandR
	End

	Event
		Type	Local
		At	UArmL
		part2	:FrozenArmor_IceChunks_Small.part
		pOther	LArmR
		pOther	HandL
	End

	Event
		Type	Local
		At	WepL
		part2	:FrozenArmor_IceChunks_Small.part
	End

	Event
		Type	Local
		At	WepR
		part2	:FrozenArmor_IceChunks_Small.part
	End
End

#############################################################

End