#########################################################
##	jack
##
FxInfo
	
Flags  InheritAlpha  # InheritAnimScale

lifespan 120

Input  
	InpName	hips
End


Condition
	On Time
	Time 0
	Event
		Type	Local
		At	Origin 
		Sound SharkBite5 95 95 .75
	End
End

Condition
	On 	Time
	Time 	0

	Event
		ename	throatspot
		Type	positonly 
		At	neck
		lookat	T_Chest
	end

	Event
		ename	attackdir
		Type	local 
		At	throatspot
		##geom	testing_target
		Bhvroverride
			PositionOffset	0 1 5
		end
	end
	Event
		ename	front
		Type	local 
		At	root
		##geom	testing_target
		Bhvroverride
			PositionOffset	0 0 15
		end
	end
end
Condition
	On 	Time
	Time 	1


	Event
		ename	sharkdir
		Type	start 
		At	attackdir
		##geom	testing_target
		Bhvroverride
			trackrate	1
		end
		magnet  front
	end	
	Event
		ename	sharkaim
		Type	positonly 
		At	neck
		##geom	testing_target
		lookat	sharkdir
	end

	Event
		ename	shark
		Type	local 
		At	sharkaim
		Anim	Shark_Spirit_Frenzy ##ent type
		Bhvroverride
			scale .3 .3 .3
			PyrRotate	0 190 30
			PositionOffset	0 -.62 .75
		end
		bhvr	:V_Mako_SpiritsharkFadeout.bhvr
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



			