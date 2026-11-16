#########################################################
##	Portal
########################################################
FxInfo



Condition

	Event
		EName	Pivot
		Type	local
		At	root
		Geom	HotFeet
		Bhvr	:Scale.bhvr
		Sound flameburst_loop 60 60 .74
		
	End

	Event
		Ename	fireSound
		Type	Local	
		At	root
		Sound fireballfly 60 60 .3
		Geom	HotFeet01
		Bhvr	:Scale.bhvr
	End

	
End

########################################################################################################

Condition
	On 	cycle
	Time 	10
	Chance	.4

	Event
		EName	Fire2
		Type	start
		At	firesound
		altpiv	2
		
		part	:EmberSputter.part
				
		Lifespan	9


	End

End

Condition
	On 	cycle
	Time 	14
	Chance	.2
	Event
		EName	Fire5
		Type	start
		At	firesound
		altpiv	5

		part	:EmberSputter.part

		Lifespan	10

	End

End


Condition
	On 	cycle
	Time 	11
	Chance	.4

	Event
		EName	Fire6
		Type	start
		At	firesound
		altpiv	6

		part	:EmberSputter.part
	
		Lifespan	8

	End

End


Condition
	On 	cycle
	Time 	15
	Chance	.4

	Event
		EName	Fire7
		Type	start
		At	firesound
		altpiv	7

		part	:EmberSputter.part

		Lifespan	9

	End

End


Condition
	On 	cycle
	Time 	18
	Chance	.4

	Event
		EName	Fire8
		Type	start
		At	firesound
		altpiv	8

		part	:EmberSputter.part

		Lifespan	10

	End

End

Condition
	On 	cycle
	Time 	12
	Chance	.4

	Event
		EName	Fire11
		Type	start
		At	firesound
		altpiv	12

		part	:EmberSputter.part
			
		Lifespan	8		

	End



End

Condition
	On 	cycle
	Time 	14
	Chance	.4

	Event
		EName	Fire11
		Type	start
		At	firesound
		altpiv	13

		part	:EmberSputter.part
			
		Lifespan	10		

	End


End

Condition
	On 	cycle
	Time 	12
	Chance	.4

	Event
		EName	Fire11
		Type	start
		At	firesound
		altpiv	14

		part	:EmberSputter.part
			
		Lifespan	9		

	End

End

########################################################################################################

Condition
	On 	cycle
	Time 	10
	Chance	.4

	Event
		EName	Fire2
		Type	start
		At	Pivot
		altpiv	2
		
		part	:EmberSputter.part

		
		Lifespan	9


	End

End


Condition
	On 	cycle
	Time 	14
	Chance	.4
	Event
		EName	Fire5
		Type	start
		At	Pivot
		altpiv	5

		part	:EmberSputter.part
	
		Lifespan	10

	End

End


Condition
	On 	cycle
	Time 	11
	Chance	.4

	Event
		EName	Fire6
		Type	start
		At	Pivot
		altpiv	6
	
		part	:EmberSputter.part

		Lifespan	8

	End

End


Condition
	On 	cycle
	Time 	15
	Chance	.4

	Event
		EName	Fire7
		Type	start
		At	Pivot
		altpiv	7
		
		part	:EmberSputter.part

		Lifespan	9

	End

End


Condition
	On 	cycle
	Time 	16
	Chance	.4

	Event
		EName	Fire8
		Type	start
		At	Pivot
		altpiv	8

		part	:EmberSputter.part

		Lifespan	10

	End

End

Condition
	On 	cycle
	Time 	12
	Chance	.4

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	12

		part	:EmberSputter.part
			
		Lifespan	8		

	End



End

Condition
	On 	cycle
	Time 	14
	Chance	.4

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	13

		part	:EmberSputter.part
			
		Lifespan	10		

	End



End

Condition
	On 	cycle
	Time 	12
	Chance	.4

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	14
		
		part	:EmberSputter.part
					
		Lifespan	9		

	End


End

#################################################################################################
#################################################################################################


Condition

	Event
		ENAME	OFFSET
		Type	local 
		At	root
		bhvr	:OffsetPortal.bhvr
		
		
	End
	
	Event
		Type	local 
		At	OFFSET
		bhvr	:Rune_spin.bhvr
		Part	:Pantheon_Runic_Emblem1.part
		
	End
	
	Event
		Type	local 
		At	OFFSET
		bhvr	:Rune_spin2.bhvr
		Part	:Pantheon_Runic_Emblem2.part
		
	End
	
	Event
		Type	local 
		At	OFFSET
		bhvr	:Rune_spin3.bhvr
		Part	:Pantheon_Runic_Emblem3.part
		
	End
	
	Event
		Type	local 
		At	OFFSET
		bhvr	:Rune_spin4.bhvr
		Part	:Pantheon_Runic_Emblem4.part
		
	End

	Event
		Type	start 
		At	OFFSET

		Part	:PortalCore.part
		Part	:PortalGasses.part
		Part	:PortalGassesDefuse.part
		
		Part	:Pantheon_Runic_Glow1.part
	End

End

End
