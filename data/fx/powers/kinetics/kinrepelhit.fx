#############################################################
## KinRepelHit.fx
#############################################################

FxInfo
Lifespan 60

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
		Sound repelhit 80 80 .7

	End
End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition
	On	Time
	Time	0

	Event
		ENAME	HitFlash1
		Type	Start
		At	Chest
		part	:KineticHitFlash01.part
		part	:KineticHitGlow01.part
		Lifespan 2

	End

	Event
		ENAME	HitSwirl1
		Type	Local
		At	Chest
		part	:KineticHitSwirl01.part
		Bhvr	:KineticSpin1.bhvr
		Lifespan 2

	End

	Event
		ENAME	HitSwirl1
		Type	Local
		At	Chest
		part	:KineticHitSwirl01.part
		Bhvr	:KineticSpin2.bhvr
		Lifespan 2

	End

	Event
		ENAME	HitSwirl1
		Type	Local
		At	Chest
		part	:KineticHitSwirl01.part
		Bhvr	:KineticSpin3.bhvr
		Lifespan 2

	End


End


End