#########################################################
##	template

FxInfo

Flags InheritAnimScale InheritAlpha

LifeSpan 1000

#### arrow in hand  ########################

Condition
	On	Time
	Time	35

	Event	
		Type	Local
		At	WepR
		Sound arrowout 50 50 .33
	End
End


Condition
	On	Time
	Time	130	

	Event
		Type	Local
		At	WepR
		Sound manyarrowsfly 90.0 90.0 .7
	End
End

Condition
	On 	Time
	Time 	35

	
	Event
		Type	Local
		At	WepR
		Bhvr	:HandRotatea.bhvr
		Geom	NormalArrow
		LifeSpan 94
	End
		
	Event
		Type	Local
		At	WepR
		Bhvr	:HandRotate1a.bhvr
		Geom	NormalArrow
		LifeSpan 94
	End
	
	Event
		Type	Local
		At	WepR
		Bhvr	:HandRotate2a.bhvr
		Geom	NormalArrow
		LifeSpan 94
	End
	
	Event
		Type	Local
		At	WepR
		Bhvr	:HandRotate3a.bhvr
		Geom	NormalArrow
		LifeSpan 96
	End
	
	Event
		Type	Local
		At	WepR
		Bhvr	:HandRotate4a.bhvr
		Geom	NormalArrow
		LifeSpan 94
	End


End



####  projectile arrow  ##############################


#Condition
#	On 	Time
#	Time 	128
#
#Event
#		EName 	Prime
#		Type	start
#		At	WepL
#		Bhvr	:projectile.bhvr
#		#Geom	NormalArrow
#		Sound 	bowtwangcone2 100.0 30.0 .7
#
#		Magnet	Target
#		LookAt	Target
#	End
#	
#End
#
Condition
	On 	PrimeHit

	Event
		Ename 	All
		Type	Destroy
	End
End

		