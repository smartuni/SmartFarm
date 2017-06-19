CREATE TABLE `smart_fences` (
  `fence_id` int(11) NOT NULL,
  `name` varchar(255) NOT NULL,
  `state` int(1) NOT NULL DEFAULT '1',
  `updated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE TABLE `smart_fences_gates` (
  `gate_id` int(11) NOT NULL,
  `fence_id` int(11) NOT NULL,
  `name` varchar(255) NOT NULL,
  `state` int(1) NOT NULL DEFAULT '1',
  `updated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE TABLE `smart_livestock` (
  `livestock_id` int(11) NOT NULL,
  `category_id` int(11) DEFAULT NULL,
  `fence_id` int(11) DEFAULT NULL,
  `name` varchar(255) NOT NULL,
  `updated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE TABLE `smart_livestock_category` (
  `category_id` int(11) NOT NULL,
  `name` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE TABLE `smart_users` (
  `user_id` int(11) NOT NULL,
  `username` varchar(100) NOT NULL,
  `password` varchar(200) NOT NULL,
  `date_loggedin` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

INSERT INTO `smart_users` (`user_id`, `username`, `password`, `date_registered`, `date_loggedin`) VALUES
(1, 'admin', 'admin', '2017-01-01 00:00:01');

ALTER TABLE `smart_fences`
  ADD PRIMARY KEY (`fence_id`);

ALTER TABLE `smart_fences_gates`
  ADD PRIMARY KEY (`gate_id`),
  ADD KEY `FK_FenceGate` (`fence_id`);

ALTER TABLE `smart_livestock`
  ADD PRIMARY KEY (`livestock_id`),
  ADD KEY `FK_LivestockCategory` (`category_id`),
  ADD KEY `FK_LivestockFence` (`fence_id`);

ALTER TABLE `smart_livestock_category`
  ADD PRIMARY KEY (`category_id`);

ALTER TABLE `smart_users`
  ADD PRIMARY KEY (`user_id`),
  ADD UNIQUE KEY `username` (`username`);

ALTER TABLE `smart_fences`
  MODIFY `fence_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1;

ALTER TABLE `smart_fences_gates`
  MODIFY `gate_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1;

ALTER TABLE `smart_livestock`
  MODIFY `livestock_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1;

ALTER TABLE `smart_livestock_category`
  MODIFY `category_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1;

ALTER TABLE `smart_users`
  MODIFY `user_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

ALTER TABLE `smart_fences_gates`
  ADD CONSTRAINT `FK_FenceGate` FOREIGN KEY (`fence_id`) REFERENCES `smart_fences` (`fence_id`);

ALTER TABLE `smart_livestock`
  ADD CONSTRAINT `FK_LivestockCategory` FOREIGN KEY (`category_id`) REFERENCES `smart_livestock_category` (`category_id`),
  ADD CONSTRAINT `FK_LivestockFence` FOREIGN KEY (`fence_id`) REFERENCES `smart_fences` (`fence_id`);
