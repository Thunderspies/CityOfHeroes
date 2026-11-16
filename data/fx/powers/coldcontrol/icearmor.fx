#########################################################
##	
##
FxInfo


Flags    InheritAlpha


##########################################################


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
		Sound IceArmor_loop 80 80 .5
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 250
	End
End







Condition
	On 	Time
	Time 	35

	Event
		Type	Local 
		At	Head
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_HEAD
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part
		part4	:Snowfall.part
		
	End
	
	Event
		Type	Local 
		At	neck
		#part1   :HeadLegschillbig.part
				
	End

End

Condition
	On 	Time
	Time 	30


	Event
		Type	Local 
		At	CHEST
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_CHEST
		# part5	:bodychillbig.part
	End
End

##############################################

Condition
	On 	Time
	Time 	25


		Event
		Type	Local 
		At	Hips
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_HIPS
		#part1	:bodychillbig.part
		#part2   :HeadLegschillbig.part
	End


	Event
		Type	Local 
		At	UARML
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_UARML
		#part1	:bodychillbig.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part

	End

	Event
		Type	Local 
		At	UARMR
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_UARMR
		#part1	powers/coldcontrol/bodychillbig.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part

	End
End


####################################################

Condition
	On 	Time
	Time 	20

		Event
		Type	Local 
		At	LARMR
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_LARMR
		#part1   powers/coldcontrol/bodychillbig.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part

	End

		Event
		Type	Local 
		At	LARML
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_LARML
		#part1   powers/coldcontrol/bodychillbig.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part

	End

		Event
		Type	Local 
		At	WepL
		part1   :Snowfall.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part
	End

		Event
		Type	Local 
		At	WepR
		part1   :Snowfall.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part

	End



End
###################################################################
Condition
	On 	Time
	Time 	15

	Event
		Type	Local 
		At	UlegR
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_ULEGR
		part1   :Snowfall.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part

	End

	Event
		Type	Local 
		At	ULEGL
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_ULEGL
		part1   :Snowfall.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part

	End
End
#################################################################


Condition
	On 	Time
	Time 	10

	Event
		Type	Local 
		At	LLEGL
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_LLEGL
		part1   :Snowfall.part
		part2	:IceArmorLegBits.part
		part3	:IceArmorLegBitsSmall.part
		#part5	:bodychillbig.part
	End

	Event
		Type	Local 
		At	LLEGR
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_LLEGR
		part1   :Snowfall.part
		part2	:IceArmorLegBits.part
		part3	:IceArmorLegBitsSmall.part
		#part5	:bodychillbig.part
	End

	Event
		Type	Local 
		At	FOOTL
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		part1	:IceArmorLegBits.part
		part2	:IceArmorLegBitsSmall.part
		
	End

	Event
		Type	Local 
		At	FOOTR
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		part1	:IceArmorLegBits.part
		part2	:IceArmorLegBitsSmall.part

	End
End

###################################################################


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
		
	End
End

Condition
	On	Time
	Time    36
	Event	
		EName 	Wind
		Type	Destroy
	End
End

Condition
	On	Time
	Time    39
	Event	
		EName 	WindFast
		Type	Destroy
	End
End

Condition
	On	Time
	Time   44
	Event	
		EName 	WindFirst
		Type	Destroy
	End

End

End			