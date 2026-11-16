#########################################################
##	jack
##
FxInfo
	
Flags  InheritAlpha  # InheritAnimScale

lifespan 130

Input  
	InpName	hips
End


Condition
	On Time
	Time 8
	Event
		Type	Local
		At	Origin 
		Sound SharkSwim2 145 145 1.0
	End
End




Condition
	On 	Time
	Time 	0
	Event
		ename	spinner
		
		Type	local 
		At	root
		Bhvroverride
			positionoffset 0 5 -2
			spinjitter .05 .05 .05
		end
	end
end

Condition
	On 	Time
	Time 	0
	
	repeat	7

	Event
		ename	charger1
		Type	local 
		At	spinner
#		geom	testing_target
		Bhvroverride
			startjitter 6 6 6
			positionoffset 0 0 0
		end
		part	:V_Mako_Spiritshark_Chargestream.part
		pmagnet chest
		lifespan 30
		lookat	target
	End

end

Condition
	On 	Time
	Time 	60

	Event
		ename	prime1
		Type	startpositonly 
		At	chest
		Bhvroverride
			trackrate 1.4
		end
		magnet	T_neck
	End
	Event
		ename	prime
		Type	startpositonly 
		At	chest
		Anim	Shark_Spirit_Charge ##ent type
		Bhvroverride
			scale .3 .3 .3
#			PyrRotate	0 190 30
#			PositionOffset	0 -.62 .75
			trackrate 1.5
		end
		bhvr	:V_Mako_SpiritsharkFade.bhvr
		magnet	T_neck
	End

#####################
	Event
		ename	chunkies
		Type	local 
		At	prime
		Bhvroverride
			PositionOffset	0 -2 0
		end
		part	:V_Mako_Spiritshark_SharkStream.part
		pmagnet chunkies
	End
	Event
		ename	streamerdorsal
		Type	local 
		At	prime
		#geom	testing_target
		animpiv	chest
		Bhvroverride
			PositionOffset	0 1.5 -1
		end
		part	:V_Mako_Spiritshark_SharkTrails.part
		part	:V_Mako_Spiritshark_SharkStream.part
		pmagnet streamerdorsal
	End
	Event
		ename	streamerTail
		Type	local 
		At	prime
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
		At	prime
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
		At	prime
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
		At	prime
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
On 	Prime1Hit 


	Event
		ename	prime1
		Type	destroy 
	End
	Event
		ename	prime
		Type	destroy 
	End
end

End	



			