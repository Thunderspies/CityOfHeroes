#########################################################
##	
##
FxInfo
##########################################################

Flags    InheritAlpha

#Condition
#	On Cycle
#	Time 30
#	Event
#		EName 	Prime
#		#At	Origin
#		Type	SetLight
#		BHVR	GENERIC\LowGlow\lightpulseclamp.bhvr
#		LifeSpan 60
#	End
#
#End
#
Condition
	On 	Time
	Time 	30


	Event
		Type	Local 
		At	CHEST
		#Bhvr 	behaviors/powers/coldcontrol/icearmorTransparrent.bhvr
		#geom	FX_IceArmor_CHEST
		part2	:HoarBits2.part
		part3	:HoarBitsSmall2.part
	End
End

##############################################

Condition
	On 	Time
	Time 	25


#	Event
#		Type	Local 
#		At	Hips
#		Bhvr 	behaviors/powers/coldcontrol/icearmorTransparrent.bhvr
#		geom	FX_IceArmor_HIPS
#		
#	End
#

	Event
		Type	Local 
		At	UARML
		#Bhvr 	behaviors/powers/coldcontrol/icearmorTransparrent.bhvr
		#geom	FX_IceArmor_UARML
		part1   :Snowfall.part
		part2	:HoarBits.part
		part3	:HoarBitsSmall.part
	End

	Event
		Type	Local 
		At	UARMR
		#Bhvr 	behaviors/powers/coldcontrol/icearmorTransparrent.bhvr
		#geom	FX_IceArmor_UARMR
		part1   :Snowfall.part
		part2	:HoarBits.part
		part3	:HoarBitsSmall.part
	End
End


####################################################

Condition
	On 	Time
	Time 	20

		Event
		Type	Local 
		At	LARMR
		#Bhvr 	behaviors/powers/coldcontrol/icearmorTransparrent.bhvr
		#geom	FX_IceArmor_LARMR
		part2	:HoarBits.part
		part3	:HoarBitsSmall.part
	End

		Event
		Type	Local 
		At	LARML
		#Bhvr 	behaviors/powers/coldcontrol/icearmorTransparrent.bhvr
		#geom	FX_IceArmor_LARML
		part2	:HoarBits.part
		part3	:HoarBitsSmall.part
	End

End
###################################################################
Condition
	On 	Time
	Time 	15

	Event
		Type	Local 
		At	UlegR
		#Bhvr 	behaviors/powers/coldcontrol/icearmorTransparrent.bhvr
		#geom	FX_IceArmor_ULEGR
		part2	:HoarLegBits.part
		part3	:HoarLegBitsSmall.part	
	End

	Event
		Type	Local 
		At	ULEGL
		#Bhvr 	behaviors/powers/coldcontrol/icearmorTransparrent.bhvr
		#geom	FX_IceArmor_ULEGL
		part2	:HoarLegBits.part
		part3	:HoarLegBitsSmall.part
	End

	Event
		Type	Local 
		At	WepL
		part1   :Snowfall.part
		part2	:HoarBits.part
		part3	:HoarBitsSmall.part
	End

		Event
		Type	Local 
		At	WepR
		part1   :Snowfall.part
		part2	:HoarBits.part
		part3	:HoarBitsSmall.part

	End

End
#################################################################
##Shines
#################################################################

Condition
	On 	Cycle
	Time 	30
	Chance	.2	

	Event
		Bhvr	behaviors/genericParticleFade.bhvr
		Type	Local 
		At	LLEGL
		part1	powers/invulnerability/FortitudeRaysSmall.part
		part2	powers/invulnerability/FortitudeRing.part
		part3	powers/invulnerability/FortitudeRing.part
		part4	powers/invulnerability/FortitudeDots.part
		LifeSpan	33
	
	End

End

Condition
	On 	Cycle
	Time 	30
	Chance	.2

	Event
		Bhvr	behaviors/genericParticleFade.bhvr
		Type	Local 
		At	LLEGR
		part1	powers/invulnerability/FortitudeRaysSmall.part
		part2	powers/invulnerability/FortitudeRing.part
		part3	powers/invulnerability/FortitudeRing.part
		part4	powers/invulnerability/FortitudeDots.part
		LifeSpan	33
	
	End
End

Condition
	On 	Cycle
	Time 	35
	Chance	.2

	Event
		Bhvr	behaviors/genericParticleFade.bhvr
		Type	Local 
		At	UArmL
		part1	powers/invulnerability/FortitudeRaysSmall.part
		part2	powers/invulnerability/FortitudeRing.part
		part3	powers/invulnerability/FortitudeRing.part
		part4	powers/invulnerability/FortitudeDots.part
		LifeSpan	33
	
	End
End

Condition
	On 	Cycle
	Time 	27
	Chance	.2

	Event
		Bhvr	behaviors/genericParticleFade.bhvr
		Type	Local 
		At	UArmR
		part1	powers/invulnerability/FortitudeRaysSmall.part
		part2	powers/invulnerability/FortitudeRing.part
		part3	powers/invulnerability/FortitudeRing.part
		part4	powers/invulnerability/FortitudeDots.part
		LifeSpan	33
	
	End

End

#################################################################


Condition

	On 	Time
	Time 	15

	Event
		Type	Local 
		At	Origin
		SOund	coldblast2 80 30 .8
	End

End

Condition

	On 	Time
	Time 	9

	Event
		Type	Local 
		At	Origin
		SOund	IcePunch1 80 30 .9
		#BHVR	GENERIC\LowGlow\lightpulseclamp.bhvr
	End

End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local 
		At	LLEGL
		Sound IceArmor_loop 80 80 .55
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 300
	
	End

	Event
		Type	Local 
		At	LLEGL
		#Bhvr 	behaviors/powers/coldcontrol/icearmorTransparrent.bhvr
		#geom	FX_IceArmor_LLEGL
		part2	:HoarLegBits.part
		part3	:HoarLegBitsSmall.part
	
	End

	Event
		Type	Local 
		At	LLEGR
		#Bhvr 	behaviors/powers/coldcontrol/icearmorTransparrent.bhvr
		#geom	FX_IceArmor_LLEGR
		part2	:HoarLegBits.part
		part3	:HoarLegBitsSmall.part
	End
	
	Event
		Type	Local 
		At	FootL
		part2	:HoarLegBits.part
		part3	:HoarLegBitsSmall.part
	
	End

	Event
		Type	Local 
		At	FootR
		
		part2	:HoarLegBits.part
		part3	:HoarLegBitsSmall.part
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