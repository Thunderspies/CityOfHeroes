#########################################################
#	RadiationAura
FxInfo

Lifespan	400

#############################################################################################

Condition
	On	Time
	Time	44

	Event	
		Type	local
		At	WepR
		Sound manyarrowshit1 80 80 .44
	End
End




Condition
	On	Time
	Time	59

	Event	
		Type	local
		At	WepR
		Sound manyarrowshit1 80 80 .77
	End
End




Condition
	On	Time
	Time	0	

	Event
		EName	ArrowHit1
		Type	start
		At	T_root
		Geom	HotFeet
		Bhvr	:ArrowRainHitFade.bhvr
		#Sound 	manyarrowsfly 90.0 30.0 .7
		Lifespan	400
	End
	
	Event
		EName	ArrowHit2
		Type	start
		At	T_root
		Geom	HotFeet01
		Bhvr	:ArrowRainHitFade.bhvr

		Lifespan	400
	End


	Event
		EName	ArrowHit3
		Type	start
		At	T_root
		Geom	HotFeet
		Bhvr	:ArrowRainHitFade.bhvr

		Lifespan	400
	End
	
	Event
		EName	ArrowHit4
		Type	start
		At	T_root
		Geom	HotFeet01
		Bhvr	:ArrowRainHitFade.bhvr

		Lifespan	400
	End

End

#######################################################################################################

Condition
	On 	time
	Time 	50

	Event
		Type	Local
		At	ArrowHit1
		altpiv	1
		
		BHVR	:ArrowRainHits.bhvr
		Geom	NormalArrow
		
		Lifespan	400


	End

	Event
		Type	Local
		At	ArrowHit1
		altpiv	2
		
		BHVR	:ArrowRainHits2.bhvr
		Geom	NormalArrow
		
		Lifespan	400


	End
	
End

Condition
	On 	Time
	Time 	53

	Event
		EName	Fire3
		Type	Local
		At	ArrowHit2
		altpiv	3

		BHVR	:ArrowRainHits3.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

	Event
		EName	Fire3
		Type	Local
		At	ArrowHit3
		altpiv	7

		BHVR	:ArrowRainHits4.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

	Event
		EName	Fire3
		Type	Local
		At	ArrowHit4
		altpiv	13

		BHVR	:ArrowRainHits.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End
End

Condition
	On 	time
	Time 	58

	Event
		EName	Fire2
		Type	Local
		At	ArrowHit3
		altpiv	9
		
		BHVR	:ArrowRainHits2.bhvr
		Geom	NormalArrow
		
		Lifespan	400


	End

	Event
		EName	Fire4
		Type	Local
		At	ArrowHit2
		altpiv	4

		BHVR	:ArrowRainHits3.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

End


Condition
	On 	time
	Time 	61

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	10

		BHVR	:ArrowRainHits4.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit4
		altpiv	13

		BHVR	:ArrowRainHits.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

End


Condition
	On 	time
	Time 	662

	Event
		EName	Fire6
		Type	Local
		At	ArrowHit3
		altpiv	6

		BHVR	:ArrowRainHits2.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit2
		altpiv	5

		BHVR	:ArrowRainHits3.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End
	
	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	12

		BHVR	:ArrowRainHits4.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End
End


Condition
	On 	time
	Time 	65

	Event
		EName	Fire7
		Type	Local
		At	ArrowHit2
		altpiv	7

		BHVR	:ArrowRainHits.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit3
		altpiv	1

		BHVR	:ArrowRainHits2.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End
End


Condition
	On 	time
	Time 	66

	Event
		EName	Fire8
		Type	Local
		At	ArrowHit4
		altpiv	8

		BHVR	:ArrowRainHits3.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	3

		BHVR	:ArrowRainHits4.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End
End


Condition
	On 	time
	Time 	68

	Event
		EName	Fire9
		Type	Local
		At	ArrowHit1
		altpiv	9

		BHVR	:ArrowRainHits.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End


	Event
		EName	Fire10
		Type	Local
		At	ArrowHit1
		altpiv	10

		BHVR	:ArrowRainHits2.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	5

		BHVR	:ArrowRainHits3.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End
End


Condition
	On 	time
	Time 	70

	Event
		EName	Fire11
		Type	Local
		At	ArrowHit1
		altpiv	11

		BHVR	:ArrowRainHits4.bhvr
		Geom	NormalArrow
		
		Lifespan	400		

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	5

		BHVR	:ArrowRainHits.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

End

Condition
	On 	time
	Time 	72

	Event
		EName	Fire11
		Type	Local
		At	ArrowHit1
		altpiv	12

		BHVR	:ArrowRainHits2.bhvr
		Geom	NormalArrow
		
		Lifespan	400		

	End


	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	12

		BHVR	:ArrowRainHits3.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

End

Condition
	On 	time
	Time 	73

	Event
		EName	Fire11
		Type	Local
		At	ArrowHit1
		altpiv	13

		BHVR	:ArrowRainHits4.bhvr
		Geom	NormalArrow
		
		Lifespan	400		

	End


	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	5

		BHVR	:ArrowRainHits2.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	9

		BHVR	:ArrowRainHits3.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End
End

Condition
	On 	time
	Time 	76

	Event
		EName	Fire11
		Type	Local
		At	ArrowHit1
		altpiv	14

		BHVR	:ArrowRainHits4.bhvr
		Geom	NormalArrow
		
		Lifespan	400		

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	5

		BHVR	:ArrowRainHits.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

End

Condition
	On 	time
	Time 	78

	Event
		EName	Fire2
		Type	Local
		At	ArrowHit2
		altpiv	1

		BHVR	:ArrowRainHits2.bhvr
		Geom	NormalArrow
		
		Lifespan	400


	End


	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	13

		BHVR	:ArrowRainHits3.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End
	
	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	2

		BHVR	:ArrowRainHits4.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

End

Condition
	On 	time
	Time 	79

	Event
		EName	Fire2
		Type	Local
		At	ArrowHit2
		altpiv	2
		

		BHVR	:ArrowRainHits.bhvr
		Geom	NormalArrow
		
		Lifespan	400


	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	6

		BHVR	:ArrowRainHits2.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End
	
	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	5

		BHVR	:ArrowRainHits3.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

End


Condition
	On 	time
	Time 	82

	Event
		EName	Fire3
		Type	Local
		At	ArrowHit2
		altpiv	3


		BHVR	:ArrowRainHits4.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End
	
	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	12

		BHVR	:ArrowRainHits.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End


End


Condition
	On 	time
	Time 	83

	Event
		EName	Fire4
		Type	Local
		At	ArrowHit2
		altpiv	10


		BHVR	:ArrowRainHits2.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	5

		BHVR	:ArrowRainHits3.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End
End


Condition
	On 	time
	Time 	85
	Event
		EName	Fire5
		Type	Local
		At	ArrowHit2
		altpiv	5

		BHVR	:ArrowRainHits4.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	1

		BHVR	:ArrowRainHits.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	1

		BHVR	:ArrowRainHits2.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

End


Condition
	On 	time
	Time 	86

	Event
		EName	Fire6
		Type	Local
		At	ArrowHit2
		altpiv	6

		BHVR	:ArrowRainHits3.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	5

		BHVR	:ArrowRainHits4.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End
	
	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	8

		BHVR	:ArrowRainHits.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

End


Condition
	On 	time
	Time 	88

	Event
		EName	Fire7
		Type	Local
		At	ArrowHit2
		altpiv	7

		BHVR	:ArrowRainHits2.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	14

		BHVR	:ArrowRainHits3.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End
	
	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	13

		BHVR	:ArrowRainHits4.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End
End


Condition
	On 	time
	Time 	91

	Event
		EName	Fire8
		Type	Local
		At	ArrowHit2
		altpiv	8

		BHVR	:ArrowRainHits.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	1

		BHVR	:ArrowRainHits2.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End
End


Condition
	On 	time
	Time 	93

	Event
		EName	Fire9
		Type	Local
		At	ArrowHit2
		altpiv	9

		BHVR	:ArrowRainHits3.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	1

		BHVR	:ArrowRainHits4.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

End


Condition
	On 	time
	Time 	95

	Event
		EName	Fire10
		Type	Local
		At	ArrowHit2
		altpiv	10

		BHVR	:ArrowRainHits.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	7

		BHVR	:ArrowRainHits2.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End
	
	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	13

		BHVR	:ArrowRainHits3.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

End


Condition
	On 	time
	Time 	98

	Event
		EName	Fire11
		Type	Local
		At	ArrowHit2
		altpiv	11


		BHVR	:ArrowRainHits4.bhvr
		Geom	NormalArrow
		
		Lifespan	400		

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	11

		BHVR	:ArrowRainHits.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End
	
	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	12

		BHVR	:ArrowRainHits2.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End
End

Condition
	On 	time
	Time 	102

	Event
		EName	Fire11
		Type	Local
		At	ArrowHit2
		altpiv	12

		BHVR	:ArrowRainHits3.bhvr
		Geom	NormalArrow
		
		Lifespan	400		

	End

	Event
		EName	Fire11
		Type	Local
		At	ArrowHit2
		altpiv	13

		BHVR	:ArrowRainHits4.bhvr
		Geom	NormalArrow
		
		Lifespan	400		

	End


	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	5

		BHVR	:ArrowRainHits.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

End

Condition
	On 	time
	Time 	104

	Event
		EName	Fire11
		Type	Local
		At	ArrowHit2
		altpiv	14

		BHVR	:ArrowRainHits2.bhvr
		Geom	NormalArrow
		
		Lifespan	400		

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	3

		BHVR	:ArrowRainHits3.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	2

		BHVR	:ArrowRainHits4.bhvr
		Geom	NormalArrow
		
		Lifespan	400

	End
End

########################################################################################################

Condition
	On	Time
	Time	0	

	Event
		EName	Hookup
		Type	start
		At	T_root
		Geom	OverHeadDummy01
		Bhvr	:ArrowRainOverheadOffsetFade.bhvr
		Lifespan	22
	End
	
End

Condition
	On 	cycle
	Time 	1
	Chance	1

	Event
		Ename	Mushroom
		Type	start
		At	Hookup
		altpiv	1
		
		ChildFx weapons/Bow/archery/ArrowNormal.fx
		Lifespan	200
	End
End

End		