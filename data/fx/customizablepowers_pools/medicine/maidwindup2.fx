#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	wepL
End

Lifespan 200

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
		Sound med5 50 50 .5
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
		Sound weld1 50 50 .6
		#Lifespan 40
	End

End


End