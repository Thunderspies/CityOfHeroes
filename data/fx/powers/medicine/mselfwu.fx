#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	wepR
End

Lifespan 100

##################################


Condition
	On	Time
	Time	22
	
	Event
		EName	Pod
		Type	Local
		At	WepR
		Geom	GEO_WepR_MediPod
		Bhvr	Behaviors/MediPodFadeIn.bhvr
		Sound med5 70 70 .8
		Lifespan 100
	End

End

Condition
	On	Time
	Time	55
	
	Event
		EName	Glow
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers/Medicine/MGlow.part
		Sound heal1 70 70 .8
		#Lifespan 40
	End

End

End