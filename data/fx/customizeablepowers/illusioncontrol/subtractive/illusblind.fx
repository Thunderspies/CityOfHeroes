#############################################################
## IllusBlind.fx
#############################################################

FxInfo
Lifespan 200

#########################################################

Input
	Inpname	Hips
End

#########################################################

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound blind 80 80 .8
	End
End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	0

	Event
		ENAME	EyeNode
		Type	Local
		At	Hair
		Geom	HairToEyesAdjustment
	End

End

Condition
	On	Time
	Time	14

	Event
		ENAME	EyeGlow
		Type	Start
		At	EyeNode
		AltPiv	1
		part	:IllusBlindOrbits01.part
		POther Neck
		Lifespan 10

	End


End

Condition
	On	Time
	Time	20

	Event
		ENAME	EyeGlow
		Type	Start
		At	EyeNode
		AltPiv	1
		part	:IllusBlindGlow02.part
		part	:IllusBlindOrbits01.part
		POther Neck
		Lifespan 10

	End


End

End