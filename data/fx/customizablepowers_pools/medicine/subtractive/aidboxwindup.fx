#########################################################
##	chill touch hit
FxInfo

Input
	InpName	wepL
End

#Lifespan 200

##################################

Condition
	On	Time
	Time	29

	Event
		Type	Local
		At	Mystic
		Sound RiktiTranspond2 70 70 .79
	End
End

Condition
	On	Time
	Time	24

	Event
		EName	Pod
		Type	Local
		At	Mystic
		Geom	GEO_WepL_MediPod
		Bhvr	Behaviors/MediPodFadeIn.bhvr
		Sound RiktiTranspond1 70 70 .60
		Part	CustomizablePowers_Pools\Medicine\Subtractive\mglow.part
	End

End

End