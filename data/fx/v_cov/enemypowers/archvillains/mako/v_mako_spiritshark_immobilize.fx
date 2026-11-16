#########################################################
##	jack
##
FxInfo

Flags  InheritAlpha  # InheritAnimScale

lifespan 200

#		Anim	Shark_Spirit_Charge ##ent type

#################### notes ###################
## Spawns 6 events in a rough arc in front of the character
## Spawns 3 shark child effects behind and to each side of the character aimed at the 6 targets

Condition
	On Time
	Time 0
	Event
		Type	Local
		At	Origin
		Sound Sharkmaterialize 140 140 .9
	End
End



Condition
	On 	Time
	Time 	0

	Event
		ename	ankle
		Type	positonly
		At	footL
#		geom	testing_target
		Bhvroverride
			stretch 1
		end
		lookat	footR
	End
	Event
		ename	halfer
		Type	local
		At	ankle
#		geom	testing_target
		Bhvroverride
			positionoffset	0 0 .5
		end
	End
	Event
		ename	troot
		Type	start
		At	root
		Bhvroverride

			positionoffset	0 0 1000
		end
	End
end


Condition
	On 	Time
	Time 	50

	Event
		ename	prime
		Type	start
		At	root
		Bhvroverride
			TrackRate 1.0
		End
		magnet Target
	End

	Event
		ename	arcscale
		Type	local
		At	root
		Bhvroverride
			scale 3 5 5
			PositionOffset	0 0 .1
		end
		lookat	T_root   ##########  ALLOWS sharks to track flying targets   ######
	End
	Event
		ename	unders
		Type	positonly
		At	halfer
#		geom	testing_target
		lookat	t_root
	End
	Event
		ename	underL
		Type	local
		At	unders
#		geom	testing_target
		Bhvroverride
			PositionOffset	-3 -.91 -10
		end
	End

	Event
		ename	underR
		Type	local
		At	unders
#		geom	testing_target
		Bhvroverride
			PositionOffset	3 -.91 -10
		end
	End

	Event
		ename	spin1
		Type	local
		At	arcscale
		##geom	testing_target
		Bhvroverride
			PositionOffset -7.8 0 6.5
		end
	End

	Event
		ename	spin2
		Type	local
		At	arcscale
		##geom	testing_target
		Bhvroverride
			PositionOffset -5.0 0 8.8
		end
	End
	Event
		ename	spin3
		Type	local
		At	arcscale
		##geom	testing_target
		Bhvroverride
			PositionOffset -1.7 0 10
		end
	End

	Event
		ename	spin4
		Type	local
		At	arcscale
		##geom	testing_target
		Bhvroverride
			PositionOffset 7.8 0 6.5
		end
	End

	Event
		ename	spin5
		Type	local
		At	arcscale
		##geom	testing_target
		Bhvroverride
			PositionOffset 5 0 8.8
		end
	End
	Event
		ename	spin6
		Type	local
		At	arcscale
		##geom	testing_target
		Bhvroverride
			PositionOffset 1.7 0 10
		end
	End


###############


#	Event
#		Type	positonly
#		At	underL
#		childfx :V_Mako_Spiritshark_Single.fx
#		lookat	spin1
#	End
	Event
		Type	positonly
		At	underL
		childfx :V_Mako_Spiritshark_Single.fx
		lookat	spin2
	End
	Event
		Type	positonly
		At	underL
		childfx :V_Mako_Spiritshark_Single.fx
		lookat	spin3
	End
#	Event
#		Type	positonly
#		At	underr
#		childfx :V_Mako_Spiritshark_Single.fx
#		lookat	spin4
#	End
	Event
		Type	positonly
		At	underr
		childfx :V_Mako_Spiritshark_Single.fx
		lookat	spin5
	End
	Event
		Type	positonly
		At	underr
		childfx :V_Mako_Spiritshark_Single.fx
		lookat	spin6
	End
End

#########  One single shark to attack air target  -steve  ##################

#
#Condition
#	On 	Time
#	Time 	50
#
#	Event
#		Type	Start
#		At	Root
#		childfx :V_Mako_Spiritshark_Single.fx
#		lookat	T_Root
#	End
#
#End
#
End



