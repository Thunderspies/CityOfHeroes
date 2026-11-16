#########################################################
##	IceBolt
############################
FxInfo

LifeSpan	200

###############################################################################
##SOUND
#############################################################

Condition
	On 	Time
	Time 	0



	Event
		EName 	fist
		Type	Local
		At	WepR
		Sound FreezeRay2 100.0 100.0 .8

	End


End

####################################################################

Condition
	On	Time
	Time	15

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part	CustomizeablePowers\ColdDomination\snowburst.part
		LifeSpan	37
	End


	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part	CustomizeablePowers\ColdDomination\ColdArmMist.part
		LifeSpan	37
	End


	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part	CustomizeablePowers\ColdDomination\ColdArmMist.part
		LifeSpan	37

	End

End
##################################################################

Condition
	On	Time
	Time	15


	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part	CustomizeablePowers\ColdDomination\IceChunksHands.part
		LifeSpan	15
	End

End

Condition
	On 	Time
	Time 	18

	Event
		EName 	Prime
		Type	Start
		At	WepR
		Bhvr	CustomizeablePowers\ColdDomination\MainIceblast3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_IceBlast
		Magnet	Target
		LookAt	Target
		Part	CustomizeablePowers\ColdDomination\FreezeRayGlowA.part
	End

	Event
		EName 	GeometryCrystals1
		Type	local
		At	Prime
		Magnet	Target
		LookAt	Target
		Bhvr	CustomizeablePowers\ColdDomination\iceblast.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_ColdBlast1
	End

	Event
		EName 	GeometryCrystals2
		Type	local
		At	Prime
		Magnet	Target
		LookAt	Target
		BhvrOverride
			TintGeom	1
		End
		Bhvr	CustomizeablePowers\ColdDomination\iceblast2.bhvr
		Geom	Tintable_ColdBlast2
	End

	Event
		EName 	trail
		Type	Start
		At	Prime
		Magnet	Target
		Bhvr	CustomizeablePowers\ColdDomination\Mainiceblast.bhvr
		Part	CustomizeablePowers\ColdDomination\FreezeRayMistA.part
		Part	CustomizeablePowers\ColdDomination\IceBoltSnowtrail2.part
		Part	CustomizeablePowers\ColdDomination\FreezeRayFrost.part
		Part	CustomizeablePowers\ColdDomination\FreezeRayFrost2.part
	End

End

##############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	start
		At	WepR
		Part	CustomizeablePowers\ColdDomination\IceBoltSnowSkatter.part
		LifeSpan	30
	End

	Event
		Type	start
		At	WepL
		Part	CustomizeablePowers\ColdDomination\IceBoltSnowSkatter.part
		LifeSpan	30
	End
End

#############################################################################################3



Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	Prime2
		Type	Destroy
	End

	Event
		Ename	trail
		Type	Destroy
	End

	Event
		Ename   IceShards
		Type	Posit
		At	target
		Part	CustomizeablePowers\ColdDomination\IceBoltMist.part
		Part	CustomizeablePowers\ColdDomination\ChillTouchSnow.part
		LifeSpan	20

	End

End


End