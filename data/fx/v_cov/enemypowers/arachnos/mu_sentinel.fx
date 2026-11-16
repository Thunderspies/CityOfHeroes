#########################################################
## Red elec blast hitting target
########################################################
FxInfo

Input  
	Inpname	Chest
End

##LifeSpan	3000000

#################################################################
##Hands
#################################################################

Condition

	On 	time
	Time 	0
	
	Event
		Type	Local
	
		At	origin

	Sound eleccage_loop 80 80 .3
	end
end


Condition

	On 	time
	Time 	0
	Event
		ename	SpinnerSlow
		Type	local
	
		At	chest
		bhvr	:mu_Sentinel_SpinSlow.bhvr
	End
	Event
		ename	Spinner
		Type	local
	
		At	SpinnerSlow
		bhvr	:mu_Sentinel_Spin.bhvr
	End
	
	Event
		ename	Rot180
		Type	Local
	
		At	Spinner
		bhvr	:mu_Sentinel_180.bhvr
	End

	Event
		ename	RotForward
		Type	Local
	
		At	Spinner
		bhvr	:mu_Sentinel_Forward.bhvr
	End

	Event
		ename	Rot180Forward
		Type	Local
	
		At	RotForward
		bhvr	:mu_Sentinel_180.bhvr
	End

	Event
		ename	RotSide
		Type	Local
	
		At	Spinner
		bhvr	:mu_Sentinel_Side.bhvr
	End

	Event
		ename	Rot180Side
		Type	Local
	
		At	RotSide
		bhvr	:mu_Sentinel_180.bhvr
	End

##########


	Event
		Type	positonly
	
		At	spinner
		part1	:mu_Sentinel_Glow.part		
	End
	Event
		Type	Local
	
		At	Spinner
		part1	:mu_SentinelFlow.part		
	End	
	Event
		Type	Local
	
		At	Rot180
		part1	:mu_SentinelFlow.part		
	End

	Event
		Type	Local
	
		At	RotForward
		part1	:mu_SentinelFlow.part		
	End	
	Event
		Type	Local
	
		At	Rot180Forward
		part1	:mu_SentinelFlow.part		
	End

	Event
		Type	Local
	
		At	RotSide
		part1	:mu_SentinelFlow.part		
	End	
	Event
		Type	Local
	
		At	Rot180Side
		part1	:mu_SentinelFlow.part		
	End

end



Condition

	On 	cycle
	Time 	1
		Chance	.05
	Event
		Type	Local
	
		At	root
		childfx	:mu_LightningBlastArc.fx
		Lifespan 10
		
	End

end

