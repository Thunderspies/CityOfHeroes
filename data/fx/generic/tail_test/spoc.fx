
FxInfo







Condition
	On 	Time
	Time 	0


	Event
		EName	spin
		type	local
		at	root
		#geom	testing_target

		bhvroverride
			PositionOffset		0.0 3 200
#			pyrrotate 45 0 0
			spin		0 0 5
			scale 1 1 1
		end

	end

	Event
		EName	trackA
		type	local
		at	spin
		#geom	testing_target

		bhvroverride
			PositionOffset		150 0.0 0.0
		end

	end


	Event
		EName	trackB
		type	local
		at	spin
		#geom	testing_target

		bhvroverride
			PositionOffset		-150 0.0 0.0
		end

	end


	Event
		EName	posA
		type	start
		at	spin
		#geom	testing_target

		bhvroverride
			trackrate	10
		end
		magnet	trackA

	end
	Event
		EName	posB
		type	start
		at	spin
		#geom	testing_target

		bhvroverride
			trackrate	10
			stretch 1
		end
		magnet	trackB
		lookat	posA

	end


	Event
		EName	targ
		type	local
		at	posB
		#geom	testing_target

		bhvroverride
			PositionOffset		0.0 0.0 .5
			scale 2 2 2
		end

	end

	Event
		EName	viso
		type	positonly
		at	head
		lookat targ

	end
	Event
		type	local
		at	viso
		Anim	FX_Skiff

		bhvroverride
			scale 1 1 1
#			pyrrotate 90 90 0
		end

	end

end
