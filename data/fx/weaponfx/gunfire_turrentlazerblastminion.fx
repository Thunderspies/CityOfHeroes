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
		geom	FireBlast1
		Part	:LazerGlowOrange.part

		LifeSpan		12

		Magnet	T_ULegR
		LookAt	T_ULegR
	End
	

	Event
		EName	Prime1
		Type	posit
		At	wepR
		Bhvr	behaviors/XRayBeam.bhvr
		geom	FireBlast1
		Part	:LazerGlowOrange.part

	LifeSpan		12

		Magnet	T_ULegL
		LookAt	T_ULegL
	End
	
	Event
		EName	Prime2
		Type	posit
		At	ToeL
		Bhvr	behaviors/XRayBeam.bhvr
		geom	FireBlast1
		Part	:LazerGlowOrange.part

		LifeSpan		12

		Magnet	T_UArmR
		LookAt	T_UArmR
	End

	Event
		EName	Prime3
		Type	posit
		At	ToeR
		Bhvr	behaviors/XRayBeam.bhvr
		geom	FireBlast1
		Part	:LazerGlowOrange.part

		LifeSpan		12

		Magnet	T_UarmL
		LookAt	T_UarmL
		
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

Condition
	On 	Prime2hit

	Event
		Ename 	Prime2
		Type	Destroy
	End

End

Condition
	On 	Prime3hit

	Event
		Ename 	Prime3
		Type	Destroy
	End

End


End				