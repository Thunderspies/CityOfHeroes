#########################################################
##	
##

FxInfo

Input  
	InpName	neck
End

Input  
	InpName	head
End

Input  
	InpName	chest
End

Input  
	InpName	hips
End

Input  
	InpName	UArmL
End

Input  
	InpName	UArmR
End

LifeSpan	50
##################################
##################################
##################################

Condition
	On 	Time
	Time 	0



	Event
		
		Type	Local
		At	chest
		Sound Crystalshit 70 70 .7
	End
End

##################################
##################################
##################################

Condition
	On 	Time
	Time 	0

	Event

		
		Type	Local
		At	chest
		Bhvr	Behaviors/QuillProjectile4.bhvr
		Geom	Fx_quill01
		
	End

	Event

		
		Type	Local
		At	chest
		Bhvr	Behaviors/QuillProjectile4.bhvr
		Geom	Fx_quill03
		
	End

	Event

		
		Type	Local
		At	chest
		Bhvr	Behaviors/QuillProjectile4.bhvr
		Geom	Fx_quill07
		
	End

	

	Event

		
		Type	Local
		At	chest
		Bhvr	Behaviors/QuillProjectile4.bhvr
		Geom	Fx_quill06
		#Sound	poisonhit2 50 30 .7
	End

	

	Event

		EName	Gas
		Type	Local
		At	chest
		#Part	:QuillsScatter.part		
		#Part1	:MustardGasHitA.part
		
		LifeSpan	30
		
	End

		
End



End				