#########################################################
##	template

FxInfo

Flags  InheritAlpha   InheritAnimScale

########  Doors  #############################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hips	
		Sound Arachnoflyer_loop 200 200 .89
		
	End
End



Condition
	On 	time
	Time 	0

	Event
		ename	Left
		Type	local
		At 	ringl
#		geom	testing_target
		bhvroverride
			positionoffset	0 -4 0
		end
	End

	Event
		ename	Right
		Type	local
		At 	ringR
#		geom	testing_target
		bhvroverride
			positionoffset	0 -4 0
		end
	End

	Event
		Type	local
		At 	ringl
#		geom	testing_target
		part	:FX_Flyer_lift.Part
		part	:FX_Flyer_fandark.Part
#		bhvroverride
#			spinjitter 0 1 0
#		end
#		lifespan	4

	End

	Event
		Type	local
		At 	ringR
#		geom	testing_target
		part	:FX_Flyer_lift.Part
		part	:FX_Flyer_fandark.Part
#		bhvroverride
#			spinjitter 0 1 0
#		end
#		lifespan	4
	End
	Event
		Type	local
		At 	ringl
#		geom	testing_target
		part	:FX_Flyer_lift.Part
		bhvroverride
#			spinjitter 0 1 0
			scale	.66 1 .66
		end
#		lifespan	4

	End

	Event
		Type	local
		At 	ringR
#		geom	testing_target
		part	:FX_Flyer_lift.Part
		bhvroverride
#			spinjitter 0 1 0
			scale	.66 1 .66
		end
#		lifespan	4
	End

end

Condition
	On 	cycle
	Time 	1
	Event
		ename	HighsprayL
		Type	startpositonly
		At 	ringl
#		geom	testing_target
		bhvr	V_COV\EnemyPowers\Arachnos\Flyer_Critter\flyer_deploy_dart.bhvr
		bhvroverride
			initialvelocity	0 -5 0
		end
		CEvent 	ENEMYFX\V_Arachnos\Critter_Flyer\FX_Flyer_dustring.fx
		CThresh 0.0001
		CDestroy 0
		lifespan	4
	End
	Event
		ename	HighsprayR
		Type	startpositonly
		At 	ringr
#		geom	testing_target
		bhvr	V_COV\EnemyPowers\Arachnos\Flyer_Critter\flyer_deploy_dart.bhvr
		bhvroverride
			initialvelocity	0 -5 0
		end
		CEvent 	ENEMYFX\V_Arachnos\Critter_Flyer\FX_Flyer_dustring.fx
		CThresh 0.0001
		CDestroy 0
		lifespan	4
	End
	Event
		ename	lowsprayL
		Type	startpositonly
		At 	left
#		geom	testing_target
		bhvr	V_COV\EnemyPowers\Arachnos\Flyer_Critter\flyer_deploy_dart.bhvr
		bhvroverride
			initialvelocity	0 -5 0
		end
		CEvent 	ENEMYFX\V_Arachnos\Critter_Flyer\FX_Flyer_dustring.fx
		CThresh 0.0001
		CDestroy 0
		lifespan	4
	End
	Event
		ename	lowsprayR
		Type	startpositonly
		At 	right
#		geom	testing_target
		bhvr	V_COV\EnemyPowers\Arachnos\Flyer_Critter\flyer_deploy_dart.bhvr
		bhvroverride
			initialvelocity	0 -5 0
		end
		CEvent 	ENEMYFX\V_Arachnos\Critter_Flyer\FX_Flyer_dustring.fx
		CThresh 0.0001
		CDestroy 0
		lifespan	4
	End



End



#Condition
#	On 	Death
#
#	Event
#		EName 	All
#		Type	Destroy
#	End
#
#End



End

