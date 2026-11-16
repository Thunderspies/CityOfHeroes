#########################################################
##	template

FxInfo

Input  
	Inpname	Root
End


#########################################################

Condition
	On 	Time
	Time 	0

	Event 
		Type	start 
		At	Root 
		Bhvr	:booster1.bhvr
		##Geom	teleport
		Part	:boosterbase.part
		Part	:boosterrings.part
		Part	:boostersparkles.part
		##Part	:boosterspinrings.part
	End
End



Condition
	On	Time
	Time	0
	
	Event	
		ENAME	SpinNode
		Type	Local
		At	Root	
		##Bhvr	:KineticScaleSmall.bhvr
		
	End

	Event	
		ENAME	Swirl1
		Type	Local
		At	SpinNode
		Bhvr	:KineticSpin1.bhvr
		
	End

	Event	
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		Bhvr	:KineticSpin2.bhvr
		
	End

	
End

Condition
	On	Cycle
	Time	15

	Event	
		ENAME	Flyaways1
		Type	Posit
		At	Swirl1
		part	:Boosterswirl.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 50
		
	End


End	

Condition
	On	Cycle
	Time	25

	
	Event	
		ENAME	Flyaways2
		Type	Posit
		At	Swirl2
		part	:Boosterswirl.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 50
		
	End


End	

End