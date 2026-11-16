#########################################################
##	template

FxInfo


#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound Fast 80 80 .8
	End
End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################

#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Condition

	On	Time
	Time	18

	Event
		Type	start
		At	chest

		Sound graniteexplo 88 88 .77

	End

	Event

		Type	start
		At	mystic
		Part	:ConeWhiteSmoke.part
		Part	:ConeWhiteDust.part
		Part	:ConeBlackDust.part

		LifeSpan	4
	End
End


Condition

	On	Time
	Time	21

	Event

		Type	start
		At	mystic
		Part	:ConeBlackBits.part

		LifeSpan	4
	End


End


End