#########################################################
#	RadiationAura
FxInfo

Lifespan	400

#############################################################################################

Condition
	On	Time
	Time	0	

	Event
		EName	ArrowHit1
		Type	start
		At	t_root
		Geom	HotFeet
		Bhvr	:RootPositionFade.bhvr
		#Sound 	manyarrowsfly 90.0 30.0 .7
		Lifespan	400
	End
End

Condition
	On	Time
	Time	1

	Event
		EName	ArrowHit2
		Type	start
		At	t_root
		Geom	HotFeet01
		Bhvr	:RootPositionFade2.bhvr

		Lifespan	400
	End

End

Condition
	On	Time
	Time	2

	Event
		EName	ArrowHit3
		Type	start
		At	t_root
		Geom	HotFeet
		Bhvr	:RootPositionFade3.bhvr

		Lifespan	400
	End
	
	Event
		EName	ArrowHit4
		Type	start
		At	t_root
		Geom	HotFeet01
		Bhvr	:RootPositionFade4.bhvr

		Lifespan	400
	End

End

#######################################################################################################

Condition
	On 	time
	Time 	2

	Event
		Type	Local
		At	ArrowHit1
		altpiv	1
		
		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400


	End

	Event
		Type	Local
		At	ArrowHit1
		altpiv	2
		
		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400


	End
	
End

Condition
	On 	Time
	Time 	3

	Event
		EName	Fire3
		Type	Local
		At	ArrowHit2
		altpiv	3

		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

	Event
		EName	Fire3
		Type	Local
		At	ArrowHit3
		altpiv	7

		BHVR	:RootScale4.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

	Event
		EName	Fire3
		Type	Local
		At	ArrowHit4
		altpiv	13

		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End
End

Condition
	On 	time
	Time 	8

	Event
		EName	Fire2
		Type	Local
		At	ArrowHit3
		altpiv	9
		
		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400


	End

	Event
		EName	Fire4
		Type	Local
		At	ArrowHit2
		altpiv	4

		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

End


Condition
	On 	time
	Time 	11

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit2
		altpiv	10

		BHVR	:RootScale4.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit4
		altpiv	13

		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

End


Condition
	On 	time
	Time 	12

	Event
		EName	Fire6
		Type	Local
		At	ArrowHit3
		altpiv	6

		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit2
		altpiv	5

		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End
	
	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	12

		BHVR	:RootScale4.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End
End


Condition
	On 	time
	Time 	15

	Event
		EName	Fire7
		Type	Local
		At	ArrowHit2
		altpiv	7

		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit3
		altpiv	1

		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End
End


Condition
	On 	time
	Time 	16

	Event
		EName	Fire8
		Type	Local
		At	ArrowHit4
		altpiv	8

		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit2
		altpiv	3

		BHVR	:RootScale4.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End
End


Condition
	On 	time
	Time 	18

	Event
		EName	Fire9
		Type	Local
		At	ArrowHit3
		altpiv	9

		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End


	Event
		EName	Fire10
		Type	Local
		At	ArrowHit4
		altpiv	10

		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit2
		altpiv	5

		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End
End


Condition
	On 	time
	Time 	20

	Event
		EName	Fire11
		Type	Local
		At	ArrowHit1
		altpiv	11

		BHVR	:RootScale4.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400		

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit2
		altpiv	5

		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

End

Condition
	On 	time
	Time 	22

	Event
		EName	Fire11
		Type	Local
		At	ArrowHit4
		altpiv	12

		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400		

	End


	Event
		EName	Fire5
		Type	Local
		At	ArrowHit3
		altpiv	12

		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

End

Condition
	On 	time
	Time 	23

	Event
		EName	Fire11
		Type	Local
		At	ArrowHit4
		altpiv	13

		BHVR	:RootScale4.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400		

	End


	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	5

		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit2
		altpiv	9

		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End
End

Condition
	On 	time
	Time 	26

	Event
		EName	Fire11
		Type	Local
		At	ArrowHit2
		altpiv	14

		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400		

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit3
		altpiv	5

		BHVR	:RootScale4.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

End

Condition
	On 	time
	Time 	28

	Event
		EName	Fire2
		Type	Local
		At	ArrowHit2
		altpiv	1

		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400


	End


	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	13

		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End
	
	Event
		EName	Fire5
		Type	Local
		At	ArrowHit4
		altpiv	2

		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

End

Condition
	On 	time
	Time 	29

	Event
		EName	Fire2
		Type	Local
		At	ArrowHit2
		altpiv	2
		

		BHVR	:RootScale4.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400


	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit4
		altpiv	6

		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End
	
	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	5

		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

End


Condition
	On 	time
	Time 	32

	Event
		EName	Fire3
		Type	Local
		At	ArrowHit2
		altpiv	3


		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End
	
	Event
		EName	Fire5
		Type	Local
		At	ArrowHit2
		altpiv	12

		BHVR	:RootScale4.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End


End


Condition
	On 	time
	Time 	33

	Event
		EName	Fire4
		Type	Local
		At	ArrowHit2
		altpiv	10


		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	5

		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End
End


Condition
	On 	time
	Time 	35
	Event
		EName	Fire5
		Type	Local
		At	ArrowHit2
		altpiv	5

		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit4
		altpiv	1

		BHVR	:RootScale4.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit2
		altpiv	1

		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

End


Condition
	On 	time
	Time 	36

	Event
		EName	Fire6
		Type	Local
		At	ArrowHit2
		altpiv	6

		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	5

		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End
	
	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	8

		BHVR	:RootScale4.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

End


Condition
	On 	time
	Time 	38

	Event
		EName	Fire7
		Type	Local
		At	ArrowHit2
		altpiv	7

		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit4
		altpiv	14

		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End
	
	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	13

		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End
End


Condition
	On 	time
	Time 	41

	Event
		EName	Fire8
		Type	Local
		At	ArrowHit2
		altpiv	8

		BHVR	:RootScale4.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit2
		altpiv	1

		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End
End


Condition
	On 	time
	Time 	43

	Event
		EName	Fire9
		Type	Local
		At	ArrowHit2
		altpiv	9

		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit3
		altpiv	1

		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

End


Condition
	On 	time
	Time 	45

	Event
		EName	Fire10
		Type	Local
		At	ArrowHit2
		altpiv	10

		BHVR	:RootScale4.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit3
		altpiv	7

		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End
	
	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	13

		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

End


Condition
	On 	time
	Time 	48

	Event
		EName	Fire11
		Type	Local
		At	ArrowHit2
		altpiv	11


		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400		

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit3
		altpiv	11

		BHVR	:RootScale4.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End
	
	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	12

		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End
End

Condition
	On 	time
	Time 	12

	Event
		EName	Fire11
		Type	Local
		At	ArrowHit2
		altpiv	12

		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400		

	End

	Event
		EName	Fire11
		Type	Local
		At	ArrowHit2
		altpiv	13

		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400		

	End


	Event
		EName	Fire5
		Type	Local
		At	ArrowHit3
		altpiv	5

		BHVR	:RootScale4.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

End

Condition
	On 	time
	Time 	4

	Event
		EName	Fire11
		Type	Local
		At	ArrowHit2
		altpiv	14

		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400		

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit1
		altpiv	3

		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End

	Event
		EName	Fire5
		Type	Local
		At	ArrowHit4
		altpiv	12

		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		#Geom	straightRoot01Light
		Geom	straightRoot01Dark
		#Geom	straightRootLight
		Geom	straightRootDark
		Part	:TreeDustFlatz.part
		Part	:TreeDust2Flatz.part
		
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
		
		Lifespan	400

	End
End

End		