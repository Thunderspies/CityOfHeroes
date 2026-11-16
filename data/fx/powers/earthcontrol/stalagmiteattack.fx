#########################################################
##	Stone_Wall
#########################################################

FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	target
End

Input  
	Inpname	mystic
End

LifeSpan	300

#########################################################


Condition
	On	Time
	Time	15
	
	Event
		EName	FootStomp
		Type	local
		At	target
		Geom	FX_RootImpact
				
	End

	
	Event
		Type	local
		At	FootStomp
		  Altpiv  1
		Part1	:StalagtiteDust.part
		Part2	:StalagtiteDust2.part
		Part3	:StalagtiteDustFlat.part
		Part4	:StalagtiteRocks01.part
	End
	
End

Condition
	On	Time
	Time	22
	Event
		EName	Rocks
		Type	start
		At	FootStomp
		  Altpiv  1
		Geom	FX_Stalagtite03
		Bhvr	behaviors/GraspingHands3.bhvr
		Part1	:StalagtiteRocks01.part
		Part2	:StalagtiteCrack2.part
	End

End


Condition
	On	Time
	Time	28

	Event
		EName	Rocks2
		Type	start
		At	FootStomp
		  Altpiv  1
		Geom	FX_Stalagtite06
		Bhvr	behaviors/GraspingHands4.bhvr
		Part2	:StalagtiteCrack2.part
	End


End

Condition
	On	Time
	Time	32

	Event
		EName	Rocks3
		Type	start
		At	FootStomp
		  Altpiv  1
		  Bhvr	behaviors/GraspingHands.bhvr
		Geom	FX_Stalagtite01
		Part2	:StalagtiteCrack2.part
	End

	
End

Condition
	On	Time
	Time	15

	Event
		EName	Rocks4
		Type	start
		At	FootStomp
		  Altpiv  1
		  Bhvr	behaviors/GraspingHands.bhvr
		Geom	FX_Stalagtite05
		Part2	:StalagtiteCrack2.part
	End

End


Condition
	On	Time
	Time	70
	
	Event
		Type	Destroy
		Ename	Rocks

	End
End

Condition
	On	Time
	Time	75

	Event
		Type	Destroy
		Ename	Rocks2

	End

End

Condition
	On	Time
	Time	72

	Event
		Type	Destroy
		Ename	Rocks3

	End

End

Condition
	On	Time
	Time	78

	Event
		Type	Destroy
		Ename	Rocks4

	End

End

############################################################
##STOMP
###########################################################
Condition
	On 	Time
	Time	21.5

	Event
		EName	FootStompx
		Type	start
		At	mystic
		Part1	:StalagtiteCrack.part
		Part2	:StalagtiteDust.part
		Part3	:StalagtiteDust2.part
		Part4	:StalagtiteDustFlat.part
		Part5	:StalagtiteRocks01.part
	End

	Event
		EName	FootStompRing
		Type	start
		At	mystic
		Part1	:Stalagtiteflash.part
		
	End


End

Condition
	On	Time
	Time	300
	
	Event
		Type	Destroy
		Ename	All

	End


End


End
