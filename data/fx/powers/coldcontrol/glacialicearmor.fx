#########################################################
##	
##
FxInfo

#Flags	IsArmor

Flags    InheritAlpha

#####################################################################################
##Lens Flares
#####################################################

Condition
	On 	cycle
	Time 	20
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	LarmL
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End
	
End

Condition
	On 	cycle
	Time 	19
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	LarmR
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End
	
End

Condition
	On 	cycle
	Time 	21
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	UarmL
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End
	
End


Condition
	On 	cycle
	Time 	22
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	UarmR
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End
	

End

Condition
	On 	cycle
	Time 	27
	Chance	.2

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End

End

Condition
	On 	cycle
	Time 	28
	Chance	.2

	Event
		EName	3
		Type	Local
		At	LlegR
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End


End

#########################################################################

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
		Sound cold_loop 80 80 .7
		bhvr	behaviors/soundFade3.bhvr
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
		Lifespan 280
	End

End





Condition
	On 	Time
	Time 	10

	Event
		Type	Local 
		At	FOOTL
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		part1	:IceArmorLegBitsA.part
		part2	:IceArmorLegBitsSmallA.part
		
	End

	Event
		Type	Local 
		At	FOOTR
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		part1	:IceArmorLegBitsA.part
		part2	:IceArmorLegBitsSmallA.part

	End
	
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

###############################################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Local 
		At	LLEGL
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_LLEGL
		part1   :Snowfall.part
		part2	:IceArmorLegBitsA.part
		part3	:IceArmorLegBitsSmallA.part
		part5	:bodychillbig.part
	End

	Event
		Type	Local 
		At	LLEGR
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_LLEGR
		part1   :Snowfall.part
		part2	:IceArmorLegBitsA.part
		part3	:IceArmorLegBitsSmallA.part
		part5	:bodychillbig.part
	End

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
		Type	Local 
		At	UlegR
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_ULEGR
		part1   :Snowfall.part
		part2	:IceArmorBitsA.part
		part3	:IceArmorBitsSmallA.part

	End

	Event
		Type	Local 
		At	ULEGL
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_ULEGL
		part1   :Snowfall.part
		part2	:IceArmorBitsA.part
		part3	:IceArmorBitsSmallA.part

	End

	Event
		EName   WindFirst
		Type	start 
		At	Root
		Bhvr 	powers/coldcontrol/IceArmorWindFirst.bhvr
		geom	FX_TmpCtrlWisp01
		
	End

	

End
###################################################################

Condition
	On 	Time
	Time 	25
	
	Event
		Type	Local 
		At	WepL
		part1   :Snowfall.part
		part2	:IceArmorBitsA.part
		part3	:IceArmorBitsSmallA.part
	End

		Event
		Type	Local 
		At	WepR
		part1   :Snowfall.part
		part2	:IceArmorBitsA.part
		part3	:IceArmorBitsSmallA.part

	End

	Event
		Type	Local 
		At	Hips
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_HIPS

		part3	:InvincibilityLightRays.part

		part1	:bodychillbig.part
		part2   :HeadLegschillbig.part
	End

	Event
		Type	Local 
		At	CHEST
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_CHEST
		 part5	:bodychillbig.part
	End

	

		
End

##########################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	Local 
		At	UARMR
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_LARMR
		part1   powers/coldcontrol/bodychillbig.part
		part2	:IceArmorBitsA.part
		part3	:IceArmorBitsSmallA.part

	End

		Event
		Type	Local 
		At	UARML
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_LARML
		part1   powers/coldcontrol/bodychillbig.part
		part2	:IceArmorBitsA.part
		part3	:IceArmorBitsSmallA.part

	End

		
End


Condition
	On 	Time
	Time 	35
	
	Event
		Type	Local 
		At	LARML
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_UARML
		part1	:bodychillbig.part
		part2	:IceArmorBitsA.part
		part3	:IceArmorBitsSmallA.part

	End

	Event
		Type	Local 
		At	LARMR
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_UARMR
		part1	:bodychillbig.part
		part2	:IceArmorBitsA.part
		part3	:IceArmorBitsSmallA.part

	End

	Event
		Type	Local 
		At	Head
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		geom	FX_IceArmor_HEAD
		part2	:IceArmorBitsA.part
		part3	:IceArmorBitsSmallA.part
		part4	:Snowfall.part
		
	End
	
	Event
		Type	Local 
		At	neck
		part1   :HeadLegschillbig.part
				
	End

End

###########################################################################

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