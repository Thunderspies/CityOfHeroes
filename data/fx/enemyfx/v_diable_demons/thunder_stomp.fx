#########################################################
##	General red electricity crawling across characters limbs
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	Mystic
End

Input  
	Inpname	HandR
End

LifeSpan	100

########################################################


Condition

	On 	time
	Time 	0
	
	Event
		Type	positonly
	
		At	handR
		Part1	:Thunder_Stomp_glow.part
	End
End

Condition

	On 	time
	Time 	24
	


	Event
		Type	local
	
		At	root
		Part1	:Thunder_Stomp_rings.part
		
		Bhvr	Behaviors/SSLSShake.bhvr
		
		Sound lightningbolt4 120 120 .9
	End
	Event
		Type	local
	
		At	footL
		childFX	:Thunder_Stomp_Branches.fx
		lifespan 100
#		geom    Testing_Target
	End

	Event
		Type	local
	
		At	footL
		childFX	:Thunder_Stomp_Branches.fx
		lifespan 100
#		geom    Testing_Target
	End

	Event
		Type	local
	
		At	footL
		childFX	:Thunder_Stomp_Branches.fx
		lifespan 100
#		geom    Testing_Target
	End

	Event
		Type	local
	
		At	footL
		childFX	:Thunder_Stomp_Branches.fx
		lifespan 100
#		geom    Testing_Target
	End

	Event
		Type	local
	
		At	footL
		childFX	:Thunder_Stomp_Branches.fx
		lifespan 100
#		geom    Testing_Target
	End

	Event
		Type	local
	
		At	footL
		childFX	:Thunder_Stomp_Branches.fx
		lifespan 100
#		geom    Testing_Target
	End

End
	

Condition

	On 	time
	Time 	23
	
	Event
		Type	local
	
		At	root
		Part1	:Thunder_Stomp_rings.part
	End
	Event
		Type	local
	
		At	footL
		childFX	:Thunder_Stomp_Branches.fx
		lifespan 100
#		geom    Testing_Target
	End

	Event
		Type	local
	
		At	footL
		childFX	:Thunder_Stomp_Branches.fx
		lifespan 100
#		geom    Testing_Target
	End

	Event
		Type	local
	
		At	footL
		childFX	:Thunder_Stomp_Branches.fx
		lifespan 100
#		geom    Testing_Target
	End
event
		Type	local
	
		At	footL
		childFX	:Thunder_Stomp_Branches.fx
		lifespan 100
#		geom    Testing_Target
	End

	Event
		Type	local
	
		At	footL
		childFX	:Thunder_Stomp_Branches.fx
		lifespan 100
#		geom    Testing_Target
	End

	Event
		Type	local
	
		At	footL
		childFX	:Thunder_Stomp_Branches.fx
		lifespan 100
#		geom    Testing_Target
	End

End


Condition

	On 	time
	Time 	27


	Event
		Type	local
		At	root
		bhvr	:Thunder_Skyhigh.bhvr
		Part	:ChargedBolt01.part
		Part	:ChargedBolt02.part
		POther HandR	
	End
End
