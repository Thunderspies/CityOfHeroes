#############################################################
## BodyGlow.fx
#############################################################

FxInfo

#########################################################
####################### AUDIO ###########################
#########################################################


Condition

	On 	Time
	Time	0

	Event
		Type Local
		At origin
#		Sound	eleccage_loop 70 30 .25
	End

End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	22

	Event
		Type	Local
		At	Chest
		Part	:ExplosiveFlash.part
		Part	:SmokeAdditive.part
		Part	:SmokeLight.part
		Part	:SmokeLight2.part
		Part	:Blast.part
		Lifespan 1
	End

	Event
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset 0.0 .5 0.0
		End
		part	:Glow.part
		Pmagnet Head
	End


	Event
		Type	Local
		At	UarmL
		part	:Glow.part
		Pmagnet UarmL
	End

	Event
		Type	Local
		At	Chest
		part	:Glow.part

		POther  Neck
	End

	Event
		Type	Local
		At	Chest
		part	:Glow.part
		POther  Hips
	End

	Event
		Type	Local
		At	Hips
		part	:Glow.part
		POther  SpadL
	End

	Event
		Type	Local
		At	Hips
		part	:Glow.part
		POther  SpadR
	End


	Event
		Type	Local
		At	Head
		part	:Glow.part
		POther  Chest
	End


	Event
		Type	Local
		At	UarmL
		part	:Glow.part
		POther  LarmL
	End

	Event
		Type	Local
		At	UarmR
		part	:Glow.part
		POther	LArmR
	End

	Event
		Type	Local
		At	UarmR
		part	:Glow.part
		POther  LarmR
	End

	Event
		Type	Local
		At	WepL
		part	:Glow.part
		POther  LarmL
	End

	Event
		Type	Local
		At	WepR
		part	:Glow.part
		POther  LarmR
	End


	Event
		Type	Local
		At	UlegL
		part	:Glow.part
		POther  Llegl
	End


	Event
		Type	Local
		At	UlegR
		part	:Glow.part
		POther  LlegR
	End

	Event
		Type	Local
		At	LLegL
		part	:Glow.part
		POther 	ToeL
	End

	Event
		Type	Local
		At	LLegR
		part	:Glow.part
		POther 	ToeR
	End

End

#########################################################

End