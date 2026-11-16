#############################################################
## RadiationEmote.fx
#############################################################


FxInfo
Lifespan 90

#############################################################
##################    BODY GLOW   ###########################
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	BodyGlow
		Type	Local
		At	Root
		ChildFX	:BodyFlash.fx
		Lifespan 90
	End


End

#############################################################
##################   BLAST AND SMOKE  #######################
#############################################################

Condition
	On	Time
	Time	5
	Event
		Ename	BodyGlow
		Type	Local
		At	Chest
		Part	:Blast_Large.part
		Part	:Blast.part
		Part	:Blast_Ring.part

		Lifespan 90
	End



	Event
		Ename	InitialBlast
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 .2 0
		End
		Part	:PlumeSmoke.part
		Part	:Shockwave.part
		Part	:GroundSmoke.part
		Lifespan 12
	End
End

#############################################################
##################   BODY AURA  #############################
#############################################################

Condition
	On 	Time
	Time 	12

	Event
		Ename	BodyGlow
		Type	Local
		At	Root
		ChildFX	:BodyGlow.fx
		Lifespan 90
	End



	Event
		Ename	LightRays
		Type	Local
		At	Chest
		Part	:RadiationLightHitLocal.part
		Lifespan 20
	End


End

#############################################################
##################   END  ###################################
#############################################################