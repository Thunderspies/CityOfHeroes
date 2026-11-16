#############################################################
## IllusDecieveHit.fx
#############################################################

FxInfo
Lifespan 150

########################################################

Input
	Inpname	Hips
End

########################################################

#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time 	8

	Event
		Type	Local
		At 	Origin
		Sound deceive1 100 100 .7
	End
End


#########################################################
###################  Hand Casts  ########################
#########################################################


Condition
	On	Time
	Time	0

	Event
		ENAME	LeftHand
		Type	Local
		At	Hair
		part	:IllusCastGlow02.part
		Lifespan 10

	End

End

Condition
	On	Time
	Time	0

	Event
		ENAME	LeftHand
		Type	Local
		At	Hair
		part	:IllusDecoyChestGlow01.part
		Lifespan 1

	End

End


End