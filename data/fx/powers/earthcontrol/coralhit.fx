#########################################################
##	Stone_Wall
#########################################################

FxInfo

Input  
	Inpname	Chest
End

Input  
	Inpname	root
End

#LifeSpan	150

##############################################################



Condition
	On	Time
	Time	5
	
	Event
		Type	start
		At	Root
		
		Part2	:FossilDust2.part
		part3	:FossilDust1.part
		#Part	:FossilRocks01.part
		Part	:FossilRocks02.part
	End
	
End

Condition
	On 	Time
	Time 	3
	Repeat	7

	Event
		Type	Local 
		At	Root
		
		BhvrOverride
			PositionOffset	0 1 0
		End
		
		ChildFX		V_COV\PhysicsEnabled\RockDebrisCoral01Scatter.fx

		Lifespan	100
		
	End

End

Condition
	On 	Time
	Time 	3
	Repeat	7

	Event
		Type	Local 
		At	Root
		
		BhvrOverride
			PositionOffset	0 1 0
		End
		
		ChildFX		V_COV\PhysicsEnabled\RockDebrisCoral02Scatter.fx

		Lifespan	100
		
	End

End

###############################################################################3
##Cracks
##################################

Condition
	On 	Time
	Time 	9
	
	Event
		EName 	Stonesickle05
		Type	start
		At	root
		Bhvr	behaviors/FossilcrackScale.bhvr
		geom	Crack01
		
		Sound explohit 100 100 .95
		
	End


End

Condition
	On	Time
	Time	10
	Event
		EName	Rocksz
		Type	start
		At	Root
		Geom	CoralFossil
		Bhvr	behaviors/FossilScale.bhvr
			
	End

End


Condition
	On	Time
	Time	9

	Event
		EName	Rocks2
		Type	start
		At	Root
		
		Part2	:FossilCrack2.part
	End


End

Condition
	On	Time
	Time	8

	Event
		EName	Rocks3
		Type	start
		At	Root
		
		Part2	:FossilCrack2.part
	End

	
End

Condition
	On	Time
	Time	11

	Event
		EName	Rocks4
		Type	start
		At	Root
		
		Part2	:FossilCrack2.part
	End

End


Condition
	On	Time
	Time	55
	
	Event
		Type	Destroy
		Ename	Rocks

	End
End

Condition
	On	Time
	Time	60

	Event
		Type	Destroy
		Ename	Rocks2

	End

End

Condition
	On	Time
	Time	58

	Event
		Type	Destroy
		Ename	Rocks3

	End

End

Condition
	On	Time
	Time	63

	Event
		Type	Destroy
		Ename	Rocks4

	End

End

End

