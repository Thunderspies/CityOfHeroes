#############################################################
## Ice Armor - Wet Ice Continuing
#############################################################

FxInfo

Flags    InheritAlpha

#############################################################

Condition

	On 	Time
	Time 	10

	Event
		Type	Local 
		At	Root
		bhvr	behaviors/soundFade3.bhvr
		Sound WetIceArmor_loop 80 80 .88
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
		part3	:RainUpwards.part
		Sound IcePunch1 80 80 .9
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
		Sound coldblast2 80 80 .7
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
	Time 	35
	
	Event
		Type	Local 
		At	neck
		part1   :Rainfall.part
	End
End

##############################################

Condition
	On 	Time
	Time 	25

	Event
		Type	Local 
		At	UARML
		part1   :RainfallArms.part

	End

	Event
		Type	Local 
		At	UARMR
		part1   :RainfallArms.part
	End
End

####################################################

Condition
	On 	Time
	Time 	20

	Event
		Type	Local 
		At	WepL
		part1   :RainfallArms.part
		
	End
		Event
		Type	Local 
		At	WepR
		part1   :RainfallArms.part
	End
End

#################################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local 
		At	LLEGL
		part1   :RainfallArms2.part
	End

	Event
		Type	Local 
		At	LLEGR
		part1   :RainfallArms2.part
	End
End

#############################################################

End			