#########################################################
##	IceBolt
############################
FxInfo

lifespan	200

##################################

Condition
	On 	Time
	Time 	0



	Event
		
		Type	Local 
		At	WepR

	
		Sound Ice8 100.0 100.0 1.0

	End


End

######################################################################################################
########################################  Emitter Barrel  ############################################
######################################################################################################

Condition
	On	Time
	Time	1

	Event	
		EName	Emitter
		Type	Local
		At	Chest
		bhvrOverride
			behavior
			PositionOffset		0 -.75 1.75
		End

#		Geom	Testing_TargetB
	End

#	Event	
#		EName	Emitter
#		Type	Local
#		At	Waist
#		bhvrOverride
#			behavior
#			PositionOffset		1.15 0 3.2
#		End
#
#		bhvr	V_COV\BaseFx\turrets\Tech\Energy_B_BarrelLeft.bhvr
##		Geom	Testing_TargetB
#	End

	Event
		EName	Eye_R
		Type	Local
		At	WepR
		bhvrOverride
			behavior
			PositionOffset		-.275 1.12 -.275
			FadeOutLength		100
		End
		Part1	V_COV\BaseFx\turrets\arcane\Cold_P_EyeGlow.part
#		Geom	Testing_TargetB
		Lifespan 10
	End

	Event
		EName	Eye_L
		Type	Local
		At	WepR
		bhvrOverride
			behavior
			PositionOffset		.375 1.12 -.275
			FadeOutLength		100
		End
		Part1	V_COV\BaseFx\turrets\arcane\Cold_P_EyeGlow.part
#		Geom	Testing_TargetB
		Lifespan 10
	End

End

######################################################################################################
#######################################  Prime Charging Both  ########################################
######################################################################################################

Condition
	On	Time
	Time	12

	Event	
		Ename   LeftHandMist
		Type	Local
		At	Emitter
		Part1	POWERS\ColdControl\snowburst.part
		
		LifeSpan	27
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	Emitter
		Part1	POWERS\ColdControl\snowburst.part
		LifeSpan	27
	End

	Event	
		Ename   LeftHandMist
		Type	Local
		At	Emitter
		
		Part2	POWERS\ColdControl\ColdArmMist.part

		LifeSpan	27
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	Emitter
		
		Part2	POWERS\ColdControl\ColdArmMist.part

		LifeSpan	27

	End

End

#######################################################################################################
#####################################  Prime Projectile Right  #######################################
######################################################################################################

#Condition
#	On 	Time
#	Time 	22
#
#	Event
#		EName 	Prime
#		Type	Start 
#		At	Emitter
#		Bhvr	behaviors/powers/coldcontrol/icebolt.bhvr
#		Geom	FX_IceBolt
#		Magnet	Target
#		LookAt	Target
#		Part1	POWERS\ColdControl\icebolttail1.part
#		Part2	POWERS\ColdControl\icebolttail1.part
#		Part3	POWERS\ColdControl\IceBoltSnowtrail.part
#		Part4	POWERS\ColdControl\IceBoltMistTrail.part
#		Part5	POWERS\ColdControl\IceBoltIceDebris2.part
#	End
#	
#End
#
#################################################################
#
#
#Condition
#	On 	Time
#	Time 	21
#
#	Event
#		EName   Flash1
#		Type	start 
#		At	Emitter
#		Part1	POWERS\ColdControl\IceBoltSnowSkatter.part
#		Lifespan 58
#		
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	23
#
#	Event
#		EName   Flash
#		Type	local 
#		At	Emitter
#		Part1	POWERS\ColdControl\IceBoltHandFlash.part
#		Part2	POWERS\ColdControl\IceBoltHandFlash2.part
#		Lifespan 65
#	End
#
#
#End
#
###############################################################
#
#
#Condition
#	On	PrimeHit
#		
#	Event
#		Ename	Prime
#		Type	Destroy
#	End
#
#	Event	
#		Ename   IceShards
#		Type	Posit
#		At	target
#		Part1	POWERS\ColdControl\IceBoltMist.part
#		Part2	POWERS\ColdControl\ChillTouchSnow.part
#		Part3   POWERS\ColdControl\ChillingTouchFlash.part
#
#		lifespan	20
#	End
#
#End

######################################################################################################
#####################################  Prime Projectile Left  ########################################
######################################################################################################

Condition
	On	Time
	Time	16

	Event
		EName	Prime
		Type	Start
		At	Emitter
		
		Bhvr	behaviors/powers/coldcontrol/icebolt.bhvr
#		Geom	Testing_TargetB
		Magnet	Target
		LookAt	Target
	End
End


Condition
	On 	Time
	Time 	19

	Event
		EName 	PrimeVisual
		Type	Start 
		At	Emitter
		Bhvr	behaviors/powers/coldcontrol/icebolt.bhvr
		#Geom	Testing_TargetB
		Geom	FX_IceBolt
		Magnet	Target
		LookAt	Target
		Part1	POWERS\ColdControl\icebolttail1.part
		Part2	POWERS\ColdControl\icebolttail1.part
		Part3	POWERS\ColdControl\IceBoltSnowtrail.part
		Part4	POWERS\ColdControl\IceBoltMistTrail.part
		Part5	POWERS\ColdControl\IceBoltIceDebris2.part
	End
	
End

################################################################


Condition
	On 	Time
	Time 	18

	Event
		EName   Flash1_L
		Type	start 
		At	Emitter
#		Part1	POWERS\ColdControl\IceBoltSnowSkatter.part
		Lifespan 58
		
	End

End

Condition
	On 	Time
	Time 	20

	Event
		EName   Flash_L
		Type	local 
		At	Emitter
		Part1	POWERS\ColdControl\IceBoltHandFlash.part
		Part2	POWERS\ColdControl\IceBoltHandFlash2.part
		Lifespan 65
	End


End

##############################################################


Condition
	On	PrimeHit
		
#	Event
#		Ename	PrimeVisual
#		Type	Destroy
#	End
	
	Event
		Ename	PrimeVisual
		Type	Destroy
	End



	Event	
		Ename   IceShards
		Type	Posit
		At	target
		Part1	POWERS\ColdControl\IceBoltMist.part
		Part2	POWERS\ColdControl\ChillTouchSnow.part
		Part3   POWERS\ColdControl\ChillingTouchFlash.part

		lifespan	20
	End

End



End				