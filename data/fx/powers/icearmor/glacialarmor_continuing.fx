#############################################################
## Ice Armor - Glacial Armor Continuing
#############################################################

FxInfo

Flags    InheritAlpha

#############################################################

Condition
	On 	Time
	Time 	0
	
	Event
		Type	Local
		At	Origin
		Sound blizzard 80 80 .9
	End
End

Condition
	On 	Time
	Time 	0
	
	Event
		Type	Local
		At	Origin
		bhvr	behaviors/soundFade3.bhvr
		Sound cold_loop 80 80 .7
		Flags	PowerLoopingSound
		Lifespan 280
	End
End

Condition
	On 	Time
	Time 	36
	
	Event
		Type	Local
		At	Origin
		Sound IceArmor3_loop 80 80 .88
		bhvr	behaviors/soundFade3.bhvr
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

###############################################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Local 
		At	LLEGL
		part5	:bodychillbig.part
	End

	Event
		Type	Local 
		At	LLEGR
		part5	:bodychillbig.part
	End
End

###################################################################

Condition
	On 	Time
	Time 	25
	
	Event
		Type	Local 
		At	Chest
		part3	:GlacialArmor_Rays1.part
		part1	:bodychillbig.part
		part2   :HeadLegschillbig.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	35
	
	Event
		Type	Local 
		At	LARML
		part1	:bodychillbig.part
	End

	Event
		Type	Local 
		At	LARMR
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		part1	:bodychillbig.part

	End
End

#############################################################

End			