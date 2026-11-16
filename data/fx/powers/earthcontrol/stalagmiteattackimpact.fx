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

LifeSpan	240

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


Condition
	On	Time
	Time	235
	
	Event
		Type	Destroy
		Ename	All

	End

End


End
