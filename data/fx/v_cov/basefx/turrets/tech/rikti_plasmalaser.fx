#########################################################
##	Turret
##

FxInfo
	
LifeSpan	200

######################################################################################################
########################################  Emitter Barrel  ############################################
######################################################################################################

Condition
	On	Time
	Time	0

	Event	
		EName	Emit_L
		Type	Local
		At	Waist

		bhvr	V_COV\BaseFx\turrets\Tech\Energy_B_BarrelLeft.bhvr
#		Geom	Testing_TargetB
		Sound riktilaserturret 110.0 110.0 .78
	End

	Event	
		EName	Emit_R
		Type	Local
		At	Waist
		bhvrOverride
			behavior
			PositionOffset		1.191 0.05 3.2
		End

		bhvr	V_COV\BaseFx\turrets\Tech\Energy_B_BarrelLeft.bhvr
#		Geom	Testing_TargetB
	End
End

######################################################################################################
###########################################   R I G H T   ############################################
######################################################################################################




Condition
	On 	Time
	Time 	0
	
	Event
		EName 	WepNode
		Type	local
		At	Emit_R
#		Bhvr	ENEMYFX\Malta\LaserFX\LaserOffset.bhvr
	End

		
End

Condition
	On 	Time
	Time 	5
	
	Event
		EName 	LaserGlowOuter
		Type	local
		At	WepNode
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserGlow1.part
		Lifespan 22

	End

		
End

Condition
	On 	Time
	Time 	10
	
	Event
		EName 	LaserGlowInner
		Type	local
		At	WepNode
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserGlowInner2.part
		Lifespan 16

	End

		
End

Condition
	On 	Time
	Time 	15
	
	Event
		EName 	LaserGlowInner
		Type	local
		At	WepNode
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserGlowStar1.part
		Lifespan 15

	End

		
End

Condition
	On 	Time
	Time 	17
	
	Event
		EName 	LaserGlowStar
		Type	local
		At	WepNode
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserGlowInner.part
		Pmagnet WepNode
		Lifespan 7

	End

		
End

Condition
	On 	Time
	Time 	30
	
	Event
		EName 	LaserGlowStar
		Type	Start
		At	WepNode
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserGlowPop.part
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserBeam04.part
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserBeam03.part
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserBeam01.part
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserBeam02.part
		POther Target
		Lifespan 2

	End

	
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	ENEMYFX\Malta\LaserFX\LaserLights.bhvr
		LifeSpan 5
	End

		
End

#Condition
#	On 	Time
#	Time 	31
#	
#	Event
#		EName 	LaserHit
#		Type	Start
#		At	Target
#		#Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserHitGlowInnerSmall.part
#		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserHitStar1Small.part
#		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserHitPopSmall.part
#		Lifespan 2
#	End
#
#		
#End

######################################################################################################
############################################   L E F  T   ############################################
######################################################################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	WepNode01
		Type	local
		At	Emit_L
#		Bhvr	ENEMYFX\Malta\LaserFX\LaserOffset.bhvr
	End

		
End

Condition
	On 	Time
	Time 	5
	
	Event
		EName 	LaserGlowOuter01
		Type	local
		At	WepNode01
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserGlow1.part
		Lifespan 22

	End

		
End

Condition
	On 	Time
	Time 	10
	
	Event
		EName 	LaserGlowInner01
		Type	local
		At	WepNode01
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserGlowInner2.part
		Lifespan 17

	End

		
End

Condition
	On 	Time
	Time 	15
	
	Event
		EName 	LaserGlowInner01
		Type	local
		At	WepNode01
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserGlowStar1.part
		Lifespan 15

	End

		
End

Condition
	On 	Time
	Time 	17
	
	Event
		EName 	LaserGlowStar01
		Type	local
		At	WepNode01
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserGlowInner.part
		Pmagnet WepNode
		Lifespan 7

	End

		
End

Condition
	On 	Time
	Time 	30
	
	Event
		EName 	LaserGlowStar01
		Type	Start
		At	WepNode01
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserGlowPop.part
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserBeam04.part
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserBeam03.part
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserBeam01.part
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserBeam02.part
		POther Target
		Lifespan 2

	End

	
	Event
		EName 	Pulse01
		Type	SetLight
		Bhvr	ENEMYFX\Malta\LaserFX\LaserLights.bhvr
		LifeSpan 5
	End

		
End

Condition
	On 	Time
	Time 	31
	
	Event
		EName 	LaserHit01
		Type	Start
		At	Target
		#Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserHitGlowInnerSmall.part
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserHitStar1Small.part
		Part	V_COV\BaseFx\turrets\Tech\Rikti_P_LaserHitPopSmall.part
		Lifespan 2

	End

		
End



End			