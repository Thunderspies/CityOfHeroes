#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	wepL
End

Lifespan 300

##################################


Condition
	On	Time
	Time	24
	
	Event
		EName	Pod
		Type	Local
		At	WepL
		Geom	GEO_WepL_MediPod
		Bhvr	Behaviors/MediPodFadeIn.bhvr
		Sound med5 70 70 .8
		#Lifespan 100
	End

End

Condition
	On	Time
	Time	42
	
	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers/Medicine/MGlow.part
		Sound xray4 70 70 .7
		#Lifespan 40
	End

End


End