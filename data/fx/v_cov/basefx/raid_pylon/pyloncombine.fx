#########################################################
##	forceBubble
########################################################
FxInfo

########## needsound:

Flags    InheritAlpha

#################################################################################
## Needsound: this file combines several other effects. It may be easy to master sound
##	to this one, but if the timing changes in the power it will need a new sound

##	SummonPylon.FX and PylonArcane.fx are the individual fx files. The Summon has a 
##		Lifespan (140 frames), pylon arcane is continuing.  

######### Disks

Condition   
	On	time
	Time	0
	Event
		EName	base
		Type	start
		At	root
		bhvroverride
			PositionOffset		0 .2 0
		end
	End
end

Condition   
	On	time
	Time	0
	Event
		Type	local
		At	base
		childfx :SummonPylon.fx
		lifespan 140
	End
end

Condition   
	On	time
	Time	65

	Event 
		Type	start
		At	base
		bhvr	:pylongrow.bhvr
		childfx :PylonArcane.fx
		magnet	base
	End
end
