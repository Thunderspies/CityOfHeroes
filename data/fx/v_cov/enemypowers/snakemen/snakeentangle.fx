#########################################################
##	
##
FxInfo

##########################################################

Condition
		
	Event
		Ename	Hookup
		Type	Local 
		At	Root
		BhvrOverride
			PositionOffset	0 0 -.5
		End

		Bhvr	:spin.bhvr
		geom	Centerdummy
				
	End

	Event
		Ename	Snakes
		Type	posit 
		At	Root
		Bhvr	:SnakeFade.bhvr
		Part	:SnakeDust.part
		Part	:SnakeDust2.part
#		Sound	Tuatha_loop 60 50 .66

	End
End

################################################################################


Condition

	Event
		Ename	Snakes
		Type	posit 
		At	FootR
		Bhvr	:SnakespinLeg.bhvr
		geom	SnakeBig02_Counter
		

	End
	
	Event
		Ename	Snakes
		Type	posit 
		At	FootR
		Bhvr	:SnakespinLegCounter.bhvr
		geom	SnakeBig02

	End
	
	Event
		Ename	Snakes
		Type	posit 
		At	FootL
		Bhvr	:SnakespinLeg.bhvr
		geom	SnakeBig02_Counter
		

	End
	
	Event
		Ename	Snakes
		Type	posit 
		At	FootL
		Bhvr	:SnakespinLegCounter.bhvr
		geom	SnakeBig02
		

	End


End

####################################################################################

Condition

	Event
		Ename	Snakes
		Type	posit 
		At	FootR
		Bhvr	:SnakespinLeg.bhvr
		geom	Snakebig01_Counter
		

	End
	
	Event
		Ename	Snakes
		Type	posit 
		At	FootR
		Bhvr	:SnakespinLegCounter.bhvr
		geom	SnakeBig01

	End
	
	Event
		Ename	Snakes
		Type	posit 
		At	FootL
		Bhvr	:SnakespinLeg.bhvr
		geom	Snakebig01_Counter
		

	End
	
	Event
		Ename	Snakes
		Type	posit 
		At	FootL
		Bhvr	:SnakespinLegCounter.bhvr
		geom	SnakeBig01
		

	End


End

######################################################################################
##Rootsnakes
#############################

Condition
	On 	Cycle
	Time	13
	Chance	.1
		

	Event
		Type	posit 
		At	Hookup
		altpiv	1
		BhvrOverride
			
			Spin			0.0 -0.1 0.0
			SpinJitter		0.0 0.052 0.0

			FadeInLength		30
			FadeOutLength		50

		End
		Bhvr	:Snakespina.bhvr
		geom	Snakebig02
		lifespan	50	
		lifespanJitter	20	
	End
End

Condition
	On 	Cycle
	Time	13
	Chance	.1
	
	
	Event
		Ename	Snakes
		Type	posit 
		At	Hookup
		altpiv	1
		
		BhvrOverride
			
			Scale			.05 .06 .05
			ScaleRate		1.0003 1.0035 1.0003
			EndScale		.06 .06 .06

			Spin			0.0 -0.1 0.0
			SpinJitter		0.0 0.053 0.0

			FadeInLength		30
			FadeOutLength		50

		End

		Bhvr	:Snakespina.bhvr
		geom	SnakeBig01
		lifespan	50	
		lifespanJitter	20	
		
	End
End

Condition
	On 	Cycle
	Time	13
	Chance	.1
	

	Event
		Ename	Snakes
		Type	posit 
		At	Hookup
		altpiv	1

		BhvrOverride
			
			Scale			.05 .04 .05
			ScaleRate		1.0003 1.0035 1.0003
			EndScale		.05 .04 .05

			Spin			0.0 -0.08 0.0
			SpinJitter		0.0 0.054 0.0

			FadeInLength		30
			FadeOutLength		50

		End
			
		Bhvr	:Snakespina.bhvr
		geom	SnakeBig
		lifespan	50	
		lifespanJitter	20	
		
	End
End

Condition
	On 	Cycle
	Time	13
	Chance	.1
	

	Event
		Ename	Snakes
		Type	posit 
		At	Hookup
		altpiv	1
		
		BhvrOverride
			
			Scale			.06 .03 .06
			ScaleRate		1.0003 1.0035 1.0003
			EndScale		.03 .03 .03

			Spin			0.0 0.02 0.0
			SpinJitter		0.0 0.012 0.0

			FadeInLength		30
			FadeOutLength		50

		End

		Bhvr	:SnakespinCounter.bhvr
		geom	SnakeBig_Counter
		lifespan	50	
		lifespanJitter	20	
		
	End
End

Condition
	On 	Cycle
	Time	13
	Chance	.1
	

	Event
		Ename	Snakes
		Type	posit 
		At	Hookup
		altpiv	1
		BhvrOverride
			
			Spin			0.0 0.18 0.0
			SpinJitter		0.0 0.031 0.0

			Scale			.03 .02 .03
			ScaleRate		1.0003 1.000135 1.0003
			EndScale		.03 .02 .03

			FadeInLength		30
			FadeOutLength		50

		End
		Bhvr	:SnakespinCountera.bhvr
		geom	SnakeBig_Counter
		lifespan	50	
		lifespanJitter	20	
		
	End
End

Condition
	On 	Cycle
	Time	13
	Chance	.1
	

	Event
		Ename	Snakes
		Type	posit
		At	Hookup
		altpiv	1
		
		BhvrOverride
			
			Spin			0.0 0.08 0.0
			SpinJitter		0.0 0.033 0.0

			Scale			.04 .05 .04
			ScaleRate		1.0003 1.000135 1.0003
			EndScale		.05 .05 .05

			FadeInLength		30
			FadeOutLength		50

		End

		Bhvr	:SnakespinCountera.bhvr
		geom	SnakeBig01_Counter
		lifespan	50	
		lifespanJitter	20	
	End
End

Condition
	On 	Cycle
	Time	13
	Chance	.1
	

	Event
		Ename	Snakes
		Type	posit
		At	Hookup
		altpiv	1
		
		BhvrOverride
			
			Spin			0.0 -0.1 0.0
			SpinJitter		0.0 0.034 0.0

			Scale			.07 .08 .07
			ScaleRate		1.0003 1.000135 1.0003
			EndScale		.08 .08 .08

			FadeInLength		30
			FadeOutLength		50

		End

		Bhvr	:Snakespina.bhvr
		geom	SnakeBig01	
		lifespan	50	
		lifespanJitter	20	
	End
End

Condition
	On 	Cycle
	Time	13
	Chance	.1
	

	Event
		Ename	Snakes
		Type	posit
		At	Hookup
		altpiv	1
		
		BhvrOverride
			
			Spin			0.0 0.1 0.0
			SpinJitter		0.0 0.051 0.0

			Scale			.07 .05 .07
			ScaleRate		1.0003 1.000135 1.0003
			EndScale		.08 .06 .08

			FadeInLength		30
			FadeOutLength		50

		End

		Bhvr	:SnakespinCountera.bhvr
		geom	SnakeBig01_Counter
		lifespan	50	
		lifespanJitter	20	
	End
End

#########################################################################


Condition
	On 	Cycle
	Time	13
	Chance	.1
	

	Event
		Ename	Snakes
		Type	posit
		At	Hookup
		altpiv	1
		
		BhvrOverride
			
			Spin			0.0 0.08 0.0
			SpinJitter		0.0 0.032 0.0

			Scale			.07 .05 .07
			ScaleRate		1.0003 1.000135 1.0003
			EndScale		.08 .05 .08

			FadeInLength		30
			FadeOutLength		50

		End

		Bhvr	:SnakespinCountera.bhvr
		geom	SnakeBig02_Counter
		lifespan	50	
		lifespanJitter	20	
	End

End



#########################################################################


Condition
	On 	Cycle
	Time	13
	Chance	.1
	

	Event
		Ename	Snakes
		Type	posit
		At	Hookup
		altpiv	1
		
		BhvrOverride
			
			Spin			0.0 -0.08 0.0
			SpinJitter		0.0 0.041 0.0

			Scale			.03 .05 .03
			ScaleRate		1.0003 1.000135 1.0003
			EndScale		.06 .06 .06

			FadeInLength		30
			FadeOutLength		50

		End

		Bhvr	:Snakespina.bhvr
		geom	SnakeBig01	
		lifespan	50	
		lifespanJitter	20	
	End
End


Condition
	On 	Cycle
	Time	13
	Chance	.1
	

	Event
		Ename	Snakes
		Type	posit
		At	Hookup
		altpiv	1
		
		BhvrOverride
			
			Spin			0.0 0.1 0.0
			SpinJitter		0.0 0.043 0.0

			Scale			.05 .04 .05
			ScaleRate		1.003 1.00135 1.003
			EndScale		.05 .07 .05

			FadeInLength		30
			FadeOutLength		50

		End

		Bhvr	:SnakespinCountera.bhvr
		geom	SnakeBig02_Counter
		lifespan	50	
		lifespanJitter	20	
	End

End

Condition
	On 	Cycle
	Time	13
	Chance	.1
	

	Event
		Ename	Snakes
		Type	posit
		At	Hookup
		altpiv	1
		
		BhvrOverride
			
			Spin			0.0 -0.1 0.0
			SpinJitter		0.0 0.043 0.0

			Scale			.04 .05 .04
			ScaleRate		1.03 1.0135 1.03
			EndScale		.04 .085 .04

			FadeInLength		30
			FadeOutLength		50

		End

		Bhvr	:Snakespina.bhvr
		geom	SnakeBig02
		lifespan	50	
		lifespanJitter	20	
	End

End

Condition
	On 	Cycle
	Time	13
	Chance	.1
	

	Event
		Ename	Snakes
		Type	posit
		At	Hookup
		altpiv	1
		
		BhvrOverride
			
			Spin			0.0 -0.08 0.0
			SpinJitter		0.0 0.05 0.0

			Scale			.05 .05 .05
			ScaleRate		1.0003 1.000135 1.0003
			EndScale		.05 .05 .05

			FadeInLength		30
			FadeOutLength		50

		End

		Bhvr	:Snakespina.bhvr
		geom	SnakeBig01
		lifespan	50	
		lifespanJitter	20	
	End
End

#######################################################################
##Cycles
########################################

Condition
	On 	Cycle
	Time	13
#	Chance	.4
	

	Event
		Type	posit 
		At	Hookup
		altpiv	1
		BhvrOverride
			
			Spin			0.0 -0.08 0.0
			SpinJitter		0.0 0.052 0.0

			Scale			.05 .05 .05
			ScaleRate		1.003 1.00135 1.003
			EndScale		.2 .2 .2

			FadeInLength		30
			FadeOutLength		50
		End
		Bhvr	:Snakespina.bhvr
		geom	Snakebig02
		lifespan	30
	End
End

Condition
	On 	Cycle
	Time	13
#	Chance	.4
	

	Event
		Type	posit 
		At	Hookup
		altpiv	1
		BhvrOverride
			
			Spin			0.0 -0.08 0.0
			SpinJitter		0.0 0.032 0.0

			Scale			.05 .05 .05
			ScaleRate		1.003 1.00135 1.003
			EndScale		.2 .2 .2

			FadeInLength		30
			FadeOutLength		50
		End
		Bhvr	:Snakespina.bhvr
		geom	Snakebig01
		lifespan	30
	End

End

Condition
	On 	Cycle
	Time	13
#	Chance	.4
	

	Event
		Type	posit 
		At	Hookup
		altpiv	1
		BhvrOverride
			
			Spin			0.0 0.08 0.0
			SpinJitter		0.0 0.042 0.0

			Scale			.05 .05 .05
			ScaleRate		1.003 1.00135 1.003
			EndScale		.2 .2 .2

			FadeInLength		30
			FadeOutLength		50
		End
		Bhvr	:Snakespina.bhvr
		geom	SnakeBig01_Counter
		lifespan	30
	End
End

Condition
	On 	Cycle
	Time	13
#	Chance	.4
	

	Event
		Type	posit 
		At	Hookup
		altpiv	1
		BhvrOverride
			
			Spin			0.0 0.08 0.0
			SpinJitter		0.0 0.042 0.0

			Scale			.05 .05 .05
			ScaleRate		1.003 1.00135 1.003
			EndScale		.2 .2 .2

			FadeInLength		30
			FadeOutLength		50
		End
		Bhvr	:Snakespina.bhvr
		geom	SnakeBig02_Counter
		lifespan	30
	End

End

End			