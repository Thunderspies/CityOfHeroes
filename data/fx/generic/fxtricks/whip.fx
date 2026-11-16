
FxInfo


##################################




Condition
	On 	Time
	Time 	0


	Event
		EName	spinner
		type	local
		at	root
		#geom testing_target
		bhvroverride
			positionoffset 1 5 2
			spin -.5 0 0
		end
	end

	Event
		EName	off
		type	local
		at	spinner
		#geom testing_target
		bhvroverride
			positionoffset 0  1 0
		end
	end

	Event
		EName	whip
		type	positonly
		at	wepr
		bhvroverride
			stretch 1
		end
		part	:whip.part
		lookat	off
	end
End

Condition
	On 	prime1hit
	Event
		EName 	all
		Type	Destroy
	End

end