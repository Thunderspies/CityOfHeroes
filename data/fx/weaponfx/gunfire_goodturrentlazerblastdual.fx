#########################################################
##	
##

FxInfo
LifeSpan  150

##################################
Condition
	On 	Time
	Time 	3

	Event
		EName	Prime
		Type	posit
		At	wepL
		Bhvr	behaviors/XRayBeam.bhvr
		geom	XrayBeam2
		Part	:LazerGlowGreen.part

		LifeSpan		12

		Magnet	T_ULegR
		LookAt	T_ULegR
	End
	

	Event
		EName	Prime1
		Type	posit
		At	wepR
		Bhvr	behaviors/XRayBeam.bhvr
		geom	XrayBeam2
		Part	:LazerGlowGreen.part

	LifeSpan		12

		Magnet	T_ULegL
		LookAt	T_ULegL
	End
	
End


#########################################################


#########################################################

Condition
	On 	Primehit
	
	Event
		Ename 	Prime
		Type	Destroy
	End

End

Condition
	On 	Prime1hit

	Event
		Ename 	Prime1
		Type	Destroy
	End

End


End				