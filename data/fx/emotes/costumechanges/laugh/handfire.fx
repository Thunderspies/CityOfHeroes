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
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	0



	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
	#	part	:BodyFire.part
		part	:FlameAdditive.part
		part	:FlameBase.part
		part	:FlameBase.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan	60
		LifespanJitter	10

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepR
	#	part	:BodyFire.part
		part	:FlameAdditive.part
		part	:FlameBase.part
		part	:FlameBase.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan	60
		LifespanJitter	10

	End

End