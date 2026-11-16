#########################################################
##	template

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
################ HEAD AND UPPER BODY ####################
#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset 0.0 .5 0.0
		End
		part	:BodySmolder.part
		part	:BodySmoke.part
		Pmagnet Head
		Lifespan 60
		LifespanJitter 20
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:BodySmolder.part
		part	:BodySmoke.part
		POther  Neck
		Lifespan 60
		LifespanJitter 20
	End

#########################################################
################## ARMS AND HANDS #######################
#########################################################

	Event
		Type	Local
		At	UarmR
		part	:BodySmolder.part
		part	:BodySmoke.part
		Pmagnet LArmR
		Lifespan 60
		LifespanJitter 20
	End

	Event
		Type	Local
		At	UarmL
		part	:BodySmolder.part
		part	:BodySmoke.part
		Pmagnet LArmL
		Lifespan 60
		LifespanJitter 20
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:BodySmolder.part
		part	:BodySmoke.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan 60
		LifespanJitter 20
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:BodySmolder.part
		part	:BodySmoke.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan 60
		LifespanJitter 20
	End

#########################################################
################ HIPS, LEGS AND FEET ####################
#########################################################

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:BodySmolder.part
		part	:BodySmoke.part
		Pmagnet UArmL
		POther  UArmL
		Lifespan 60
		LifespanJitter 20
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:BodySmolder.part
		part	:BodySmoke.part
		Pmagnet LlegR
		POther  LlegR
		Lifespan 60
		LifespanJitter 20
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:BodySmolder.part
		part	:BodySmoke.part
		Pmagnet Llegl
		POther  Llegl
		Lifespan 60
		LifespanJitter 20
	End

	Event
		Type	Local
		At	FootR
		part	:BodySmolder.part
		part	:BodySmoke.part
		Pmagnet LLegR
		Lifespan 60
		LifespanJitter 20
	End


	Event
		Type	Local
		At	FootL
		part	:BodySmolder.part
		part	:BodySmoke.part
		Pmagnet LLegL
		Lifespan 60
		LifespanJitter 20
	End


	Event
		Type	Local
		At	LlegR
		part	:BodySmolder.part
		part	:BodySmoke.part
		Pmagnet FootR
		POther  FootR
		Lifespan 60
		LifespanJitter 20
	End

	Event
		Type	Local
		At	LlegL
		part	:BodySmolder.part
		part	:BodySmoke.part
		Pmagnet FootL
		POther  FootL
		Lifespan 60
		LifespanJitter 20
	End
End