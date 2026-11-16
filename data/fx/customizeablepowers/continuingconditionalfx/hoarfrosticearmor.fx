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
		part2	:HoarBits2.part
		part3	:HoarBitsSmall2.part
	End
End

##############################################

Condition
	On 	Time
	Time 	25




	Event
		Type	Local
		At	UARML
		part1   :Snowfall.part
		part2	:HoarBits.part
		part3	:HoarBitsSmall.part
	End

	Event
		Type	Local
		At	UARMR
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
		part2	:HoarBits.part
		part3	:HoarBitsSmall.part
	End

		Event
		Type	Local
		At	LARML
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
		part2	:HoarLegBits.part
		part3	:HoarLegBitsSmall.part
	End

	Event
		Type	Local
		At	ULEGL
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
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part
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
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part
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
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part
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
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part
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
		Sound IcePunch1 80 80 .9
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
		part2	:HoarLegBits.part
		part3	:HoarLegBitsSmall.part

	End

	Event
		Type	Local
		At	LLEGR
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
		BhvrOverride
			TintGeom	1
			Alpha		50
		End
		geom	Tintable_FX_TmpCtrlWisp01
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
		BhvrOverride
			TintGeom	1
			Alpha		50
		End
		geom	Tintable_FX_TmpCtrlWisp01
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
		BhvrOverride
			TintGeom	1
			Alpha		50
		End
		geom	Tintable_FX_TmpCtrlWisp01

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