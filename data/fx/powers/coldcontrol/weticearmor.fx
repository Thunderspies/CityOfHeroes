#########################################################
##	
##
FxInfo

Flags    InheritAlpha

##########################################################

Condition
	On 	Time
	Time 	35

	Event
		Type	Local 
		At	Head
		Bhvr 	behaviors/powers/coldcontrol/icearmorLowerAlpha.bhvr
		geom	FX_IceArmor_HEAD
						
	End
	
	Event
		Type	Local 
		At	neck
		part1   :Rainfall.part
				
	End

End

Condition
	On 	Time
	Time 	30


	Event
		Type	Local 
		At	CHEST
		Bhvr 	behaviors/powers/coldcontrol/icearmorLowerAlpha.bhvr
		geom	FX_IceArmor_CHEST
		
	End
End

##############################################

Condition
	On 	Time
	Time 	25


		Event
		Type	Local 
		At	Hips
		Bhvr 	behaviors/powers/coldcontrol/icearmorLowerAlpha.bhvr
		geom	FX_IceArmor_HIPS
		
	End


	Event
		Type	Local 
		At	UARML
		Bhvr 	behaviors/powers/coldcontrol/icearmorLowerAlpha.bhvr
		geom	FX_IceArmor_UARML
		part1   :RainfallArms.part

	End

	Event
		Type	Local 
		At	UARMR
		Bhvr 	behaviors/powers/coldcontrol/icearmorLowerAlpha.bhvr
		geom	FX_IceArmor_UARMR
		part1   :RainfallArms.part

	End
End


####################################################

Condition
	On 	Time
	Time 	20

		Event
		Type	Local 
		At	LARMR
		Bhvr 	behaviors/powers/coldcontrol/icearmorLowerAlpha.bhvr
		geom	FX_IceArmor_LARMR
		
	End

		Event
		Type	Local 
		At	LARML
		Bhvr 	behaviors/powers/coldcontrol/icearmorLowerAlpha.bhvr
		geom	FX_IceArmor_LARML
		
	End

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
###################################################################
Condition
	On 	Time
	Time 	15

	Event
		Type	Local 
		At	UlegR
		Bhvr 	behaviors/powers/coldcontrol/icearmorLowerAlpha.bhvr
		geom	FX_IceArmor_ULEGR
		
	End

	Event
		Type	Local 
		At	ULEGL
		Bhvr 	behaviors/powers/coldcontrol/icearmorLowerAlpha.bhvr
		geom	FX_IceArmor_ULEGL
		
	End
End
#################################################################


Condition
	On 	Time
	Time 	10

	Event
		Type	Local 
		At	LLEGL
		Bhvr 	behaviors/powers/coldcontrol/icearmorLowerAlpha.bhvr
		geom	FX_IceArmor_LLEGL
		part1   :RainfallArms2.part
		Sound WetIceArmor_loop 80 80 .88
	End

	Event
		Type	Local 
		At	LLEGR
		Bhvr 	behaviors/powers/coldcontrol/icearmorLowerAlpha.bhvr
		geom	FX_IceArmor_LLEGR
		part1   :RainfallArms2.part
	End

End
#################################################################


Condition

	On 	Time
	Time 	15

	Event
		Type	Local 
		At	Origin
		SOund	coldblast2 80 30 .7
	End

End

Condition

	On 	Time
	Time 	9

	Event
		Type	Local 
		At	Origin
		SOund	IcePunch1 80 30 .9
	End

End

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