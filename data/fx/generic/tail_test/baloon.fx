
FxInfo


##################################




Condition
	On 	Time
	Time 	0


	Event
		EName	handot
		type	positonly
		at handL

	end

	Event
		EName	hando
		type	local
#		Geom	testing_target
		at handot
		bhvroverride
			PositionOffset		0.0 1 0.0
		end

	end


	Event
		EName	prime
		Type	start
		At	handot
#		Geom	testing_target
		bhvroverride
			PositionOffset		0.0 10 0.0
			trackrate	1.05
			InitialVelocity	0 1 0
		end


		magnet  hando


		End

	Event
		EName	string
		Type	positonly
		At	handot
#		Geom	testing_target
		Geom	XrayBeam
		bhvroverride
scale .2 .2 1
		end


		lookat prime


		End

	Event
		EName	baloon
		Type	local
		At	string
		Geom	Prop_golfball
		bhvroverride
			PositionOffset		0 0 5
			scale 5 5 1

		end
	end


end

Condition
	On 	primehit
	
	Event
		EName 	prime
		Type	Destroy
	End
	Event
		EName 	prime
		Type	Destroy
	End
	Event
		EName 	string
		Type	Destroy
	End
	Event
		EName 	baloon
		Type	Destroy
	End
	Event
		EName 	hando
		Type	Destroy
	End

	Event
		EName	driftspot ## spot to drift to
		Type	start
		At	root
#		Geom	testing_target
		bhvroverride
			PositionOffset		0 30 0
			StartJitter		20 0 20
		end
	end

	Event
		EName	prime1  
		Type	start
		At	handot
#		Geom	testing_target
		bhvroverride
			trackrate .1
#			InitialVelocityJitter	.1 .1 .1
		end
		magnet driftspot
	end
	
	Event
		EName	prime2
		Type	positonly
		At	prime1
		bhvr	:baloonfade.bhvr

#		Geom	testing_target
		Geom	XrayBeam

	end
	Event
		ename   baloon2
		Type	local
		At	prime2
		Geom	Prop_golfball
		bhvr	:baloonfade.bhvr
		bhvroverride
			PositionOffset		0 0 5
			scale 5 5 1			
			PyrRotate	0 0 0

		end
	end

end

Condition
	On 	prime1hit
	Event
		EName 	all
		Type	Destroy
	End

end