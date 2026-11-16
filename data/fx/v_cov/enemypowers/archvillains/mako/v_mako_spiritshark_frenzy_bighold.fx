#########################################################
##	jack
##
FxInfo
	
Flags  InheritAlpha  # InheritAnimScale

##lifespan 120



Condition
	On 	Time
	Time 	5
	
	Event
		Type	Local
		At 	origin
		Sound Jaws1 80 80 .9
	End
End

Condition
	On 	Time
	Time 	0
	
	Event
		Type	Local
		At 	origin
		Sound sharkbite_loop 80 80 .7
	End
End

######## simulated water splash  #####################################################

Condition
	On 	Time
	Time 	2

	Event
		Type	Start
		At	Root
		childfx Volumes\Water\DeepSplash\Encounter_DeepSplash.fx
	End
End


##Condition
##	On 	Time
##	Time 	4
##
##	Event
##		Type	Start
##		At	Root
##		childfx V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_Spiritshark_Circle_Hit.fx
##	End
##End
##
##########  Shark  ######################################################


Condition
	On 	Time
	Time 	1



	Event
		ename	shark
		Type	local 
		At	Root  
		Anim	Shark_Spirit_Hold
##		Bhvr	Behaviors/SharkBite_Scale.bhvr
		Bhvr	:V_Mako_SpiritsharkFadeout.bhvr
		Bhvroverride
##			scale 1 1 1
			PyrRotate	90 0 0  
			PositionOffset	0 4 0
		end
		
	End
End




Condition
	On 	Time
	Time 	1 #60

	Event
		ename	chunkies
		Type	local 
		At	shark

		Bhvroverride
			PositionOffset	0 0 -10
		end
		part	:V_Mako_Spiritshark_SharkStream_back.part
		pmagnet chunkies
	End

	Event
		ename	streamerdorsal
		Type	local 
		At	shark
		#geom	testing_target
		animpiv	chest
		Bhvroverride
			PositionOffset	0 1.5 -1
		end
		part	:V_Mako_Spiritshark_SharkTrails.part
		pmagnet prime
	End
	Event
		ename	streamerTail
		Type	local 
		At	shark
		#geom	testing_target
		animpiv	ToeL
		Bhvroverride
			PositionOffset	0 .9 -1.5
		end
		part	:V_Mako_Spiritshark_SharkTrails.part
		pmagnet prime
	End
	
	Event
		ename	streamerTail2
		Type	local 
		At	shark
		#geom	testing_target
		animpiv	footL
		Bhvroverride
			PositionOffset	0 -1.5 -1.8
			pyrrotate	0 0 180
		end
		part	:V_Mako_Spiritshark_SharkTrails.part
		pmagnet prime
	End
	Event
		ename	streamerFinL
		Type	local 
		At	shark
		#geom	testing_target
		animpiv	UarmL
		Bhvroverride
			PositionOffset	0 -2.5 -1
			pyrrotate	30 0 180
		end
		part	:V_Mako_Spiritshark_SharkTrails.part
		pmagnet prime
	End
	Event
		ename	streamerFinR
		Type	local 
		At	shark
		#geom	testing_target
		animpiv	UarmR
		Bhvroverride
			PositionOffset	0 -2.5 -1
			pyrrotate	30 0 180
		end
		part	:V_Mako_Spiritshark_SharkTrails.part
		pmagnet prime
	End

End


Condition
	On 	cycle
	Time 	21

	Event
		Type	local 
		At	shark
		animpiv	jaw
		Bhvroverride
			pyrrotate	0 180 0
		end
		part	:V_Mako_Spiritshark_Mouthburst.part
		pmagnet chunkies
		lifespan 60
	End

	Event
		Type	local 
		At	shark
		animpiv	jaw
		part	:V_Mako_Spiritshark_Mouthburst.part
		pmagnet chunkies
		lifespan 60
	End


end

Condition
	On 	cycle
	Time 	16

	Event
		Type	local 
		At	shark
		animpiv	jaw
		Bhvroverride
			pyrrotate	0 180 0
		end
		part	:V_Mako_Spiritshark_Mouthburst.part
		pmagnet chunkies
		lifespan 60
	End

	Event
		Type	local 
		At	shark
		animpiv	jaw
		part	:V_Mako_Spiritshark_Mouthburst.part
		pmagnet chunkies
		lifespan 60
	End


end

End	



			